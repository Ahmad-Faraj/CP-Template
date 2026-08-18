#requires -Version 7.0
<#
.SYNOPSIS
    Verifies every template against the rules in .claude/CLAUDE.md.

.EXAMPLE
    pwsh tools/check.ps1              # whole repo
    pwsh tools/check.ps1 -Path DP     # one directory
    pwsh tools/check.ps1 -NoCompile   # contract checks only, instant
#>
[CmdletBinding()]
param(
    [string] $Path = '',
    [switch] $NoCompile,
    [int]    $Jobs = 8
)

$ErrorActionPreference = 'Stop'
$repo = Split-Path -Parent $PSScriptRoot

# Never checked: frozen archive, prose notes, tooling, editor config.
$skipTop = @('other', 'notes', 'tools', 'verify', 'tests', '.git', '.claude', '.vscode')

$root = if ($Path) { Join-Path $repo $Path } else { $repo }
if (-not (Test-Path $root)) { throw "no such path: $root" }

$files = Get-ChildItem $root -Recurse -File -Filter *.cpp | Where-Object {
    $top = ($_.FullName.Substring($repo.Length + 1) -split '[\\/]')[0]
    $skipTop -notcontains $top
} | Sort-Object FullName

if (-not $files) { Write-Host "no .cpp files under $root"; exit 0 }

$required = @('Use when:', 'Handles:', 'Time:', 'Indexing:')

$results = $files | ForEach-Object -ThrottleLimit $Jobs -Parallel {
    $repo      = $using:repo
    $required  = $using:required
    $noCompile = $using:NoCompile

    $rel   = $_.FullName.Substring($repo.Length + 1)
    $errs  = [System.Collections.Generic.List[string]]::new()
    $warns = [System.Collections.Generic.List[string]]::new()

    $lines = @(Get-Content -LiteralPath $_.FullName)
    $text  = $lines -join "`n"

    # Header: the leading run of // lines, before anything else.
    $header = @()
    foreach ($l in $lines) { if ($l -like '//*') { $header += $l } else { break } }

    if (-not $header) {
        $errs.Add('no header comment')
    } elseif ($header[0] -notmatch '^//\s*\S.*:\s*\S') {
        $errs.Add('first line must be "// <Name>: <what it does>"')
    }
    $htext = $header -join "`n"
    foreach ($k in $required) {
        if ($htext -notmatch [regex]::Escape($k)) { $errs.Add("header missing '$k'") }
    }

    # Mandatory usage function.
    if ($text -notmatch '(?m)^\s*\w[\w:<>,\s\*&]*\bsolve\s*\(') { $errs.Add('no solve() usage') }

    # A block comment holding statements is a dead-code graveyard.
    if ($text -match '(?s)/\*.*?[;{].*?\*/') { $warns.Add('commented-out code block') }

    $long = @($lines | Where-Object { $_.Length -gt 120 }).Count
    if ($long) { $warns.Add("$long line(s) over 120 cols") }

    if (-not $noCompile) {
        $out = & g++ -std=c++17 -O2 -Wall -fsyntax-only $_.FullName 2>&1
        if ($LASTEXITCODE -ne 0) {
            $first = @($out | Where-Object { $_ -match 'error:' } | Select-Object -First 1)
            $errs.Add("does not compile: $first")
        } else {
            $n = @($out | Where-Object { $_ -match 'warning:' }).Count
            if ($n) { $warns.Add("$n compiler warning(s)") }
        }
    }

    [pscustomobject]@{ File = $rel; Errors = $errs.ToArray(); Warnings = $warns.ToArray() }
}

$results = @($results | Sort-Object File)
$bad     = @($results | Where-Object { $_.Errors.Count })
$warned  = @($results | Where-Object { -not $_.Errors.Count -and $_.Warnings.Count })

foreach ($r in $bad) {
    Write-Host "FAIL  $($r.File)" -ForegroundColor Red
    $r.Errors   | ForEach-Object { Write-Host "        $_" }
    $r.Warnings | ForEach-Object { Write-Host "        (warn) $_" -ForegroundColor DarkYellow }
}
foreach ($r in $warned) {
    Write-Host "WARN  $($r.File)" -ForegroundColor Yellow
    $r.Warnings | ForEach-Object { Write-Host "        $_" }
}

$clean = $results.Count - $bad.Count - $warned.Count
Write-Host ''
Write-Host "$($results.Count) checked  |  $clean clean  |  $($warned.Count) warn  |  $($bad.Count) fail"

if ($bad.Count) { exit 1 } else { exit 0 }
