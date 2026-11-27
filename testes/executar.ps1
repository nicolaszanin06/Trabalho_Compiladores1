$ErrorActionPreference = 'Stop'

$diretorioTestes = Split-Path -Parent $MyInvocation.MyCommand.Path
$diretorioProjeto = Split-Path -Parent $diretorioTestes
$identificador = [guid]::NewGuid().ToString('N')
$executavel = Join-Path $diretorioTestes ("scanner-teste-{0}.exe" -f $identificador)
$pastaTemporariaC = Join-Path $env:TEMP ("jtoc-c-teste-{0}" -f $identificador)
$sucesso = 'Analise concluida com sucesso! Nenhum erro sintatico encontrado.'

$casos = @(
    @{ Codigo = 0; Nome = 'arquivo vazio'; Arquivo = 'validos/vazio.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Codigo = 0; Nome = 'declaracoes'; Arquivo = 'validos/declaracoes.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Codigo = 0; Nome = 'ifs aninhados'; Arquivo = 'validos/ifs_aninhados.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Codigo = 0; Nome = 'espacos e comentarios'; Arquivo = 'validos/comentarios.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Codigo = 1; Nome = 'expressao ausente'; Arquivo = 'invalidos/expressao_ausente.java'; Saida = ''; Erro = "Erro sintatico na linha 1: syntax error perto de ';'" },
    @{ Codigo = 1; Nome = 'texto solto'; Arquivo = 'invalidos/texto_solto.java'; Saida = ''; Erro = "Erro sintatico na linha 1: syntax error perto de ','" },
    @{ Codigo = 1; Nome = 'sem ponto e virgula'; Arquivo = 'invalidos/sem_ponto_e_virgula.java'; Saida = ''; Erro = "Erro sintatico na linha \d+: syntax error perto de '.*'" },
    @{ Codigo = 0; Nome = 'atribuicao na declaracao'; Arquivo = 'validos/atribuicao_na_declaracao.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Codigo = 0; Nome = 'condicao com identificador'; Arquivo = 'validos/condicao_com_identificador.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Codigo = 1; Nome = 'caractere invalido'; Arquivo = 'invalidos/caractere_invalido.java'; Saida = "Erro lexico na linha \d+: Caractere invalido '@'"; Erro = '' },
    @{ Codigo = 1; Nome = 'string nao encerrada'; Arquivo = 'invalidos/string_nao_encerrada.java'; Saida = 'Erro lexico na linha 1: String nao encerrada'; Erro = '' },
    @{ Codigo = 1; Nome = 'string multilinha'; Arquivo = 'invalidos/string_multilinha.java'; Saida = 'Erro lexico na linha 1: String nao encerrada'; Erro = "Erro sintatico na linha 2: syntax error perto de 'linha'" },
    @{ Codigo = 1; Nome = 'char multilinha'; Arquivo = 'invalidos/char_multilinha.java'; Saida = "Erro lexico na linha 1: Caractere invalido '''`nErro lexico na linha 2: Caractere invalido '''"; Erro = "Erro sintatico na linha 2: syntax error perto de ';'" },
    @{ Codigo = 1; Nome = 'comentario nao encerrado'; Arquivo = 'invalidos/comentario_nao_encerrado.java'; Saida = 'Erro lexico na linha 1: Comentario nao encerrado'; Erro = '' }
)

$casosIR = @(
    @{ Codigo = 0; Nome = 'IR aritmetica'; Entrada = 'int x = 3 + 4;'; Saida = 'LOAD_CONST 3`nLOAD_CONST 4`nADD`nSTORE x'; Erro = '' },
    @{ Codigo = 0; Nome = 'IR minimo inteiro'; Entrada = 'int x = -2147483648;'; Saida = 'LOAD_CONST -2147483648`nSTORE x'; Erro = '' },
    @{ Codigo = 0; Nome = 'IR matematica e impressao'; Entrada = 'System.out.println(Math.sqrt(9));'; Saida = 'LOAD_CONST 9`nI2F`nSQRT`nPRINTLN'; Erro = '' },
    @{ Codigo = 1; Nome = 'IR overflow inteiro'; Entrada = 'int x = 2147483648;'; Saida = ''; Erro = '[\s\S]*fora da faixa de 32 bits[\s\S]*' },
    @{ Codigo = 1; Nome = 'IR escape invalido'; Entrada = 'System.out.print("\q");'; Saida = ''; Erro = '[\s\S]*Escape invalido[\s\S]*' }
)

function Normalizar-Saida([string]$texto) {
    return $texto.Replace("`r`n", "`n").TrimEnd("`n")
}

try {
    & gcc -o $executavel (Join-Path $diretorioProjeto 'src/jtoc.tab.c') `
        (Join-Path $diretorioProjeto 'src/lex.yy.c') `
        (Join-Path $diretorioProjeto 'src/ast.c') `
        (Join-Path $diretorioProjeto 'src/tabela_simbolos.c') `
        (Join-Path $diretorioProjeto 'src/gerador_intermediario.c') `
        (Join-Path $diretorioProjeto 'src/gerador_c.c')
    if ($LASTEXITCODE -ne 0) {
        throw 'Falha ao compilar o compilador. Verifique se o gcc esta instalado e se Flex/Bison foram regenerados.'
    }

    $falhas = 0
    foreach ($caso in $casos) {
        $arquivo = Join-Path $diretorioTestes $caso.Arquivo
        $processoInfo = New-Object System.Diagnostics.ProcessStartInfo
        $processoInfo.FileName = $executavel
        $processoInfo.Arguments = '--debug "' + $arquivo + '"'
        $processoInfo.UseShellExecute = $false
        $processoInfo.RedirectStandardOutput = $true
        $processoInfo.RedirectStandardError = $true

        $processo = [System.Diagnostics.Process]::Start($processoInfo)
        $saida = Normalizar-Saida $processo.StandardOutput.ReadToEnd()
        $erro = Normalizar-Saida $processo.StandardError.ReadToEnd()
        $processo.WaitForExit()

        $saidaCorreta = $saida -cmatch "\A(?:$($caso.Saida))\z"
        if ($caso.Codigo -eq 0) {
            $saidaCorreta = $saida -cmatch "\A$([regex]::Escape($sucesso))\nAST:\nBloco\n[\s\S]*\nTabela de simbolos \(valor de inicializacao\):\n[\s\S]*\z"
        }
        $erroCorreto = $erro -cmatch "\A(?:$($caso.Erro))\z"
        if ($saidaCorreta -and $erroCorreto -and $processo.ExitCode -eq $caso.Codigo) {
            Write-Host "OK: $($caso.Nome)"
        } else {
            $falhas++
            Write-Host "FALHOU: $($caso.Nome)"
            Write-Host "  stdout: $saida"
            Write-Host "  stderr: $erro"
        }
        $processo.Dispose()
    }

    foreach ($caso in $casosIR) {
        $processoInfo = New-Object System.Diagnostics.ProcessStartInfo
        $processoInfo.FileName = $executavel
        $processoInfo.Arguments = '--ir'
        $processoInfo.UseShellExecute = $false
        $processoInfo.RedirectStandardInput = $true
        $processoInfo.RedirectStandardOutput = $true
        $processoInfo.RedirectStandardError = $true
        $processo = [System.Diagnostics.Process]::Start($processoInfo)
        $processo.StandardInput.Write($caso.Entrada)
        $processo.StandardInput.Close()
        $saida = Normalizar-Saida $processo.StandardOutput.ReadToEnd()
        $erro = Normalizar-Saida $processo.StandardError.ReadToEnd()
        $processo.WaitForExit()
        $esperado = $caso.Saida.Replace('`n', "`n")
        if ($processo.ExitCode -eq $caso.Codigo -and
            $saida -cmatch "\A(?:$([regex]::Escape($esperado)))\z" -and
            $erro -cmatch "\A(?:$($caso.Erro))\z") {
            Write-Host "OK: $($caso.Nome)"
        } else {
            $falhas++
            Write-Host "FALHOU: $($caso.Nome)"
            Write-Host "  stdout: $saida"
            Write-Host "  stderr: $erro"
        }
        $processo.Dispose()
    }

    # Traduz, compila e executa pelo menos um programa C completo.
    New-Item -ItemType Directory -Path $pastaTemporariaC | Out-Null
    $entradaC = 'int soma = 0; for (int i = 0; i < 5; i++) { soma += i; } System.out.println(soma); System.out.println(Math.sqrt(9));'
    $processoInfo = New-Object System.Diagnostics.ProcessStartInfo
    $processoInfo.FileName = $executavel
    $processoInfo.Arguments = '--c'
    $processoInfo.UseShellExecute = $false
    $processoInfo.RedirectStandardInput = $true
    $processoInfo.RedirectStandardOutput = $true
    $processoInfo.RedirectStandardError = $true
    $processo = [System.Diagnostics.Process]::Start($processoInfo)
    $processo.StandardInput.Write($entradaC)
    $processo.StandardInput.Close()
    $codigoC = $processo.StandardOutput.ReadToEnd()
    $erroTraducao = $processo.StandardError.ReadToEnd()
    $processo.WaitForExit()

    $arquivoC = Join-Path $pastaTemporariaC 'programa.c'
    $executavelC = Join-Path $pastaTemporariaC 'programa.exe'
    [System.IO.File]::WriteAllText($arquivoC, $codigoC, [System.Text.UTF8Encoding]::new($false))

    if ($processo.ExitCode -eq 0 -and $erroTraducao -eq '' -and $codigoC.Contains('int main(void)')) {
        & gcc -std=c11 -Wall -Wextra -Wpedantic $arquivoC -o $executavelC -lm
        if ($LASTEXITCODE -eq 0) {
            $saidaC = & $executavelC
            $saidaCTexto = Normalizar-Saida ($saidaC -join "`n")
            if ($saidaCTexto -eq "10`n3.000000") {
                Write-Host 'OK: geracao C, compilacao GCC e execucao'
            } else {
                $falhas++
                Write-Host "FALHOU: execucao do C gerado: $saidaCTexto"
            }
        } else {
            $falhas++
            Write-Host 'FALHOU: GCC nao compilou o C gerado.'
        }
    } else {
        $falhas++
        Write-Host "FALHOU: traducao C: $erroTraducao"
    }
    $processo.Dispose()

    $total = $casos.Count + $casosIR.Count + 1
    Write-Host "`n$($total - $falhas)/$total testes passaram."
    if ($falhas -gt 0) { exit 1 }
} finally {
    if (Test-Path -LiteralPath $executavel) {
        Remove-Item -LiteralPath $executavel
    }
    if (Test-Path -LiteralPath $pastaTemporariaC) {
        Remove-Item -LiteralPath $pastaTemporariaC -Recurse -Force
    }
}
