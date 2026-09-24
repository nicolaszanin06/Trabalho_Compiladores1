$ErrorActionPreference = 'Stop'

$diretorioTestes = Split-Path -Parent $MyInvocation.MyCommand.Path
$diretorioProjeto = Split-Path -Parent $diretorioTestes
$executavel = Join-Path $diretorioTestes ("scanner-teste-{0}.exe" -f [guid]::NewGuid().ToString('N'))
$sucesso = 'Analise concluida com sucesso! Nenhum erro sintatico encontrado.'

$casos = @(
    @{ Nome = 'arquivo vazio'; Arquivo = 'validos/vazio.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Nome = 'declaracoes'; Arquivo = 'validos/declaracoes.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Nome = 'ifs aninhados'; Arquivo = 'validos/ifs_aninhados.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Nome = 'espacos e comentarios'; Arquivo = 'validos/comentarios.java'; Saida = [regex]::Escape($sucesso); Erro = '' },
    @{ Nome = 'sem ponto e virgula'; Arquivo = 'invalidos/sem_ponto_e_virgula.java'; Saida = ''; Erro = "Erro sintatico na linha \d+: syntax error perto de '.*'" },
    @{ Nome = 'atribuicao nao suportada'; Arquivo = 'invalidos/atribuicao_na_declaracao.java'; Saida = ''; Erro = "Erro sintatico na linha \d+: syntax error perto de '.*'" },
    @{ Nome = 'condicao nao suportada'; Arquivo = 'invalidos/condicao_nao_suportada.java'; Saida = ''; Erro = "Erro sintatico na linha \d+: syntax error perto de '.*'" },
    @{ Nome = 'caractere invalido'; Arquivo = 'invalidos/caractere_invalido.java'; Saida = "Erro lexico na linha \d+: Caractere invalido '@'\n$([regex]::Escape($sucesso))"; Erro = '' },
    @{ Nome = 'string nao encerrada'; Arquivo = 'invalidos/string_nao_encerrada.java'; Saida = "Erro lexico na linha \d+: String nao encerrada\n$([regex]::Escape($sucesso))"; Erro = '' }
)

function Normalizar-Saida([string]$texto) {
    return $texto.Replace("`r`n", "`n").TrimEnd("`n")
}

try {
    & gcc -o $executavel (Join-Path $diretorioProjeto 'src/scanner.tab.c') (Join-Path $diretorioProjeto 'src/lex.yy.c')
    if ($LASTEXITCODE -ne 0) {
        throw 'Falha ao compilar o scanner. Verifique se o gcc esta instalado.'
    }

    $falhas = 0
    foreach ($caso in $casos) {
        $arquivo = Join-Path $diretorioTestes $caso.Arquivo
        $processoInfo = New-Object System.Diagnostics.ProcessStartInfo
        $processoInfo.FileName = $executavel
        $processoInfo.Arguments = '"' + $arquivo + '"'
        $processoInfo.UseShellExecute = $false
        $processoInfo.RedirectStandardOutput = $true
        $processoInfo.RedirectStandardError = $true

        $processo = [System.Diagnostics.Process]::Start($processoInfo)
        $saida = Normalizar-Saida $processo.StandardOutput.ReadToEnd()
        $erro = Normalizar-Saida $processo.StandardError.ReadToEnd()
        $processo.WaitForExit()

        $saidaCorreta = $saida -cmatch "\A(?:$($caso.Saida))\z"
        $erroCorreto = $erro -cmatch "\A(?:$($caso.Erro))\z"
        if ($saidaCorreta -and $erroCorreto) {
            Write-Host "OK: $($caso.Nome)"
        } else {
            $falhas++
            Write-Host "FALHOU: $($caso.Nome)"
            Write-Host "  stdout: $saida"
            Write-Host "  stderr: $erro"
        }
    }

    Write-Host "`n$($casos.Count - $falhas)/$($casos.Count) testes passaram."
    if ($falhas -gt 0) { exit 1 }
} finally {
    if (Test-Path -LiteralPath $executavel) {
        Remove-Item -LiteralPath $executavel
    }
}
