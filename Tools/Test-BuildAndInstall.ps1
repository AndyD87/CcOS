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
$SolutionDir = $CurrentDir+"\Solution"

$VisualStudios  = @("2017", "2015", "2013")
$Architectures  = @("x86", "x64")
$Configurations = @("Release", "Debug",  "RelWithDebInfo", "MinSizeRel")


$TestLog     = $CurrentDir+"\Test.log" 
if((Test-Path $TestLog))
{
    Remove-Item $TestLog
}

$VisualStudio
Import-Module ..\ThirdParty\Powershell-Common\VisualStudio.ps1 -Force

foreach($VisualStudio in $VisualStudios)
{
    foreach($Architecture in $Architectures)
    {
        foreach($Configuration in $Configurations)
        {
            VisualStudio-GetEnv -Version $VisualStudio -Architecture $Architecture 
            $env:CL = "/MP"
            if((Test-Path $SolutionDir))
            {
                Remove-Item $SolutionDir -Recurse -Force
            }
            New-Item $SolutionDir -ItemType Directory
            cd $SolutionDir
            & "cmake.exe" "../../" "-G" "NMake Makefiles" "-DCMAKE_BUILD_TYPE=$Configurations"
            if($LASTEXITCODE -ne 0)
            {
                cd $CurrentDir
                $Msg = "Failed: cmake generation with $VisualStudio $Architecture $Configuration"
                Add-Content $TestLog $Msg
                if($StopOnError) { throw $Msg }
            }
            & "nmake"
            if($LASTEXITCODE -ne 0)
            {
                cd $CurrentDir
                $Msg = "Failed: cmake generation with $VisualStudio $Architecture $Configuration"
                Add-Content $TestLog $Msg
                if($StopOnError) { throw $Msg }
            }
            & "nmake" "install"
            cd $CurrentDir
            Add-Content $TestLog "Success: $VisualStudio $Architecture $Configuration"
            return 0;
        }
    }
}
