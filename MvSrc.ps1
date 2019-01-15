$sWorkingDir = Get-Location 

Function MoveSrc
{
    PARAM(
        $sDir
    )
    cd $sDir
    $oItems = Get-ChildItem "$sDir\src"
    foreach($oItem in $oItems)
    {
        $sSource = "src\"+$oItem.Name
        git mv $sSource $sDir
    }
    Remove-Item "src" -Recurse -Force
}

Function FindSrc
{
    PARAM(
        $sDir
    )
    $oItems = Get-ChildItem -Directory $sDir
    foreach($oItem in $oItems)
    {
        if($oItem.Name -eq "src")
        {
            MoveSrc $sDir
        }
        else
        {
            [string]$sName = $oItem.Name
            FindSrc ($sDir + "\" + $sName)
        }
    }
}

FindSrc $sWorkingDir.Path
