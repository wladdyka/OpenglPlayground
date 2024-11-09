# Define directories to scan
$directories = @("src/", "shaders/")

# Define file types to include
$fileExtensions = @("*.cpp", "*.h", "CMakeLists.txt", ".glsl")

# Temporary file to hold combined content
$tempFile = [System.IO.Path]::GetTempFileName()

# Function to check if a file is relevant
function Is-Relevant($filename) {
    # Skip generated files and non-project files
    if ($filename -match "build/" -or
        $filename -match "CMakeFiles/" -or
        $filename -match "\.git/" -or
        $filename -match "cmake-build-debug/") {
        return $false
    }
    return $true
}

# Loop through each directory
foreach ($dir in $directories) {
    if (Test-Path -Path $dir) {
        # Find files with the specified extensions
        foreach ($ext in $fileExtensions) {
            Get-ChildItem -Path $dir -Recurse -Filter $ext | ForEach-Object {
                $file = $_.FullName
                # Check if the file is relevant
                if (Is-Relevant -filename $file) {
                    Write-Host "Processing $file"
                    Add-Content -Path $tempFile -Value "`n`n--- $file ---`n"
                    Get-Content -Path $file | Add-Content -Path $tempFile
                }
            }
        }
    } else {
        Write-Host "Directory $dir does not exist."
    }
}

# Copy combined content to clipboard
Get-Content -Path $tempFile | Set-Clipboard

# Cleanup
Remove-Item -Path $tempFile

Write-Host "All relevant files have been combined and copied to the clipboard."
