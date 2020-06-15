# Please run the script at the root of source directory.
# Usage:
#   anvil.ps1 [-build-type <Debug|Release>] [-no-build]

param (
  [string]${build-type} = "Release",
  [switch]${no-build} = $false
)

# CPM currently requires absolute path to work correctly.
function QueryDepsSrcCacheDir {
  $cache_dir = [IO.Path]::Combine($env:USERPROFILE, "cpp-cpm-cache")
  if ($env:ANVIL_DEPS_CACHE) {
    $cache_dir = $env:ANVIL_DEPS_CACHE
  }
  return [IO.Path]::GetFullPath($cache_dir)
}

$GENERATOR = "Visual Studio 16 2019"
$OUT_DIR = "msvc"
$DEPS_SRC_CACHE = QueryDepsSrcCacheDir

Write-Output "[*] Params:"
  "  GENERATOR=$GENERATOR"
  "  DEPS_SOURCE_CACHE=$DEPS_SRC_CACHE"
  "  -build-type=${build-type}"
  "  -no-build=${no-build}"

$root_dir = Get-Location

$out_path = [IO.Path]::Combine($root_dir, "build", $OUT_DIR)
if (!(Test-Path $out_path)) {
  New-Item -Path $out_path -ItemType Directory
}

cmake `
  -DCPM_SOURCE_CACHE="$DEPS_SRC_CACHE" `
  -G $GENERATOR `
  -B $out_path `
  -S $root_dir

if (${no-build}) {
    Write-Output "[*] Generate project only, skip build"
    exit
}

cmake `
  --build $out_path `
  --config ${build-type}
