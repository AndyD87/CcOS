
<#
.SYNOPSIS
    Check if file contains a specified string
.DESCRIPTION
    Finding string in file is case sensitive.
    CommandLet will use Get-Content to parse each line and stop on first match
.PARAMETER FilePath
    Path to File where strings should be replaced
.PARAMETER String
    String to find in File
.EXAMPLE
    FileContains Test.txt "TestString"
#>
Function FileContains
{
    PARAM( 
        [PARAMETER(Mandatory)]
        $FilePath,
        [PARAMETER(Mandatory)]
        $String
    )
    $FileContent = Get-Content $FilePath
    foreach($Line in $FileContent)
    {
        if($Line -cmatch $String)
        {
            return $true
        }
    }
    return $false
}

<#
.SYNOPSIS
    Replace String in File
.DESCRIPTION
    Replace String in File case sensitive.
    CommandLet will use Get-Content to parse each line and Set-Content to write result
.PARAMETER FilePath
    Path to File where strings should be replaced
.PARAMETER String
    String to find in File
.PARAMETER WorkingDir
    String to be replaced with in file
.EXAMPLE
    FileReplace Test.txt "TestString" "Replacement"
#>
Function FileReplace
{
    PARAM( 
        [PARAMETER(Mandatory)]
        $FilePath,
        [PARAMETER(Mandatory)]
        $String,
        [PARAMETER(Mandatory)]
        $Replace
    )
    $FileContent = Get-Content $FilePath
    $NewFileContent = @()
    foreach($Line in $FileContent)
    {
        $NewLine = $Line -creplace $String, $Replace
        $NewFileContent += $NewLine
    }
    Set-Content $FilePath $NewFileContent
}

<#
.SYNOPSIS
    Format a CamelCase String to an UPPER_CASE String.
.DESCRIPTION
    Add an underline on each upper character or digit in string, except the firt.
    Example Outputs:
    CamelCase -> CAMEL_CASE
    camelCase -> CAMEL_CASE
    Camel2Case -> CAMEL_2_CASE
.PARAMETER String
    String to format into new string
.EXAMPLE
    CamelCaseToUpper "CamleCaseString"
#>
Function CamelCaseToUpper
{
    PARAM(
        [PARAMETER(Mandatory)]
        [string]$String
    )

    [char[]]$Chars = $String
    $Count = 0
    $NewString = ""
    foreach($Char in $Chars)
    {
        if( [char]::IsUpper($Char) -or
            [char]::IsDigit($Char))
        {
            if($Count -ne 0)
            {
                $Append = "_" + $Char
            }
            else
            {
                $Append = $Char
            }
        }
        else
        {
            $Append = [char]::ToUpper($Char)
        }
        $Count++
        $NewString += $Append
    }
    Return $NewString
}

<#
.SYNOPSIS
    Simple to upper script
.DESCRIPTION
.PARAMETER String
    String to format into new string
.EXAMPLE
    DefaultToUpper "CamleCaseString"
#>
Function DefaultToUpper
{
    PARAM(
        [PARAMETER(Mandatory)]
        [string]$String
    )

    [char[]]$Chars = $String
    $Count = 0
    $NewString = ""
    foreach($Char in $Chars)
    {
        $Append = [char]::ToUpper($Char)
        $Count++
        $NewString += $Append
    }
    Return $NewString
}

<#
.SYNOPSIS
    Remove lines in files
.DESCRIPTION
    Finding string in file on every and remove or replace it.
.PARAMETER FilePath
    Path to File where lines shoul be removed
.PARAMETER String
    String to find in File
.PARAMETER Replace
    Optinal Paramter to write line if found
.PARAMETER Replace
    Optinal Paramter, if true Replace will be executed only once per file.
.EXAMPLE
    FileContains Test.txt "TestString"
#>
Function FileRemoveMatchingLine
{
    PARAM( 
        [PARAMETER(Mandatory)]
        $FilePath,
        [PARAMETER(Mandatory)]
        $String,
        $Replace = $null,
        $ReplaceOnce = $false
    )
    $bChanged = $false

    $FileContent = Get-Content $FilePath
    $NewFileContent = @()
    foreach($Line in $FileContent)
    {
        if($Line -cmatch $String)
        {
            if($Replace -ne $null)
            {
                $NewFileContent += $Replace
                if($ReplaceOnce)
                {
                    $Replace = $null
                }
            }
            $bChanged = $true;
        }
        else
        {
            $NewFileContent += $Line
        }
    }
    if($bChanged)
    {
        Set-Content $FilePath $NewFileContent
    }

    return $bChanged
}

<#
.SYNOPSIS
    This is an example to walk through directories and filter for files.
    To find target file, add addtional else if for $Item.

.PARAMETER Path
    Path to recursive file operation.
#>
Function FileRecursive
{
    PARAM( 
        [PARAMETER(Mandatory)]
        $Path
    )
    $Items = Get-ChildItem $Path
    foreach($Item in $Items)
    {
        # Ignore git directory
        if($Item.Name -eq ".git")
        {
            continue
        }
        # Ignore this script if it is within this directories
        elseif($PSCommandPath -eq $Item.FullName)
        {
            continue
        }
        # Call recursive if it is container
        elseif($Item.PSIsContainer)
        {
            [string]$FileFullPath = $Item.FullName
            # Ignore Cache directory
            if($FileFullPath.EndsWith("Cache"))
            {
                continue
            }
            else
            {
                FileRecursive $FileFullPath
            }
        }
        else
        {
            [string]$FileFullPath = $Item.FullName
            if( $FileFullPath.EndsWith(".cpp") -or
                $FileFullPath.EndsWith(".c") -or
                $FileFullPath.EndsWith(".h") -or
                $FileFullPath.EndsWith(".h.in")
            )
            {
                $bChangedPage     = FileRemoveMatchingLine $FileFullPath " * @page" " * @file" $true
                $bChangedSubpage  = FileRemoveMatchingLine $FileFullPath " * @subpage"
                if($bChangedPage -or $bChangedSubpage)
                {
                    Write-Host $Item.FullName
                }

            }
        }
    }
}

# Example call on current directory
FileRecursive ..\