# Common Powershell Commandlets

This commandlets are used from several Projects.

## Import one or more Commandlets

For importing all Commanlets execute:

    Import-Module "All.ps1"
    
For importing a specific set of Commdlets, like Git-XXX, execute:

    Import-Module "Git.ps1"
    
## Update Sources

This repository is often copied to other locations.  
To keep it uptodate and get latest version from [github](https://github.com/AndyD87/Powershell-Common.git) just execute:

    & 'UpdateCommon.ps1'
    
It requires git for cloning the repository.
