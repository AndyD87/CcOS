#cd ..
#mkdir Solution.VC12
#cd Solution.VC12
#cmake ../ -G "Visual Studio 12" -DCMAKE_GENERATOR_PLATFORM=x64
#cmake.exe --build . --config Release
#ctest -C Release
#cd ..
#cd Tools
PARAM(
    [bool]$StopOnError = $false
)

$CurrentDir  = (Get-Item .\).FullName
$TestLog     = $CurrentDir+"\Test.log" 
$SolutionDir = $PSScriptRoot+"\Solution"
$OutputDir   = $PSScriptRoot+"\Output"
$CcOSRootDir = $PSScriptRoot+"\.."

$VisualStudios = @("Visual Studio 12", "Visual Studio 14", "Visual Studio 15") #
$Architectures  = @("win32", "x64")
$Configurations = @("Debug", "Release") # Not required but possible to test : "RelWithDebInfo", "MinSizeRel")


$TestLog     = $CurrentDir+"\Test.log" 
if((Test-Path $TestLog))
{
    Remove-Item $TestLog
}

foreach($VisualStudio in $VisualStudios)
{
    foreach($Architecture in $Architectures)
    {
        foreach($Configuration in $Configurations)
        {
            # Fist Clean Solution if Existing
            if((Test-Path $SolutionDir))
            {
                Remove-Item $SolutionDir -Recurse -Force
            }
            # Fist Clean Solution if Existing
            if((Test-Path $OutputDir))
            {
                Remove-Item $OutputDir -Recurse -Force
            }
            New-Item $SolutionDir -ItemType Directory
            New-Item $OutputDir -ItemType Directory
            cd $SolutionDir

            $VisualStudioString = $VisualStudio
            if($Architecture -eq "x64")
            {
                $VisualStudioString += " Win64"
            }
            try
            {
                & "cmake.exe" "$CcOSRootDir" "-G" $VisualStudioString "-DCCOS_OUTPUT_DIR=`"$OutputDir`""
                if($LASTEXITCODE -ne 0)
                {
                    cd $CurrentDir
                    $Msg = "Failed: cmake generation with $VisualStudioString $Configuration"
                    Add-Content $TestLog $Msg
                    throw $Msg
                }
                cmake.exe --build . --config $Configuration
                if($LASTEXITCODE -ne 0)
                {
                    cd $CurrentDir
                    $Msg = "Failed: cmake build with $VisualStudioString $Configuration"
                    Add-Content $TestLog $Msg
                    throw $Msg
                }
                ctest -C $Configuration
                if($LASTEXITCODE -ne 0)
                {
                    cd $CurrentDir
                    $Msg = "Failed: ctest with $VisualStudioString $Configuration"
                    Add-Content $TestLog $Msg
                    throw $Msg
                }
                Add-Content $TestLog "Success: $VisualStudioString $Configuration"
            }
            catch
            {
                Write-Host $_.Exception.Message
            }
            finally
            {
                if($StopOnError){ throw $Msg }
                cd $CurrentDir
            }
        }
    }
}
