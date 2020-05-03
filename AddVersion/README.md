# AddVersion

A small tool used for visual studio project versoin management.

[reference](http://support.microsoft.com/?id=237870)

File "VersionNo.h" must exsists in project directory, content must be like "major.minor.revision.build", e.g. "1.0.0.2345".

After execute the command, it will generate a file names "VersionNo.ini" in folder "Debug" or "Release" based on your current project setting.

The file "VersionNo.ini" could be used in nsis bat file to make an installer.

## Usage

```cmd
AddVersion [versionNo.h path] [versionNo.ini path] [section] [number]
section: M m r b
M: major version
m: minor version
r: revision version
b: build times
number: add by number, 1~100 for version increment, 0 for version decrement 1.
```

## Sample

1. AddMajorVersion

	`AddVersion.exe $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini M 1`

2. AddMinorVersion

	`AddVersion.exe $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini m 1`

3. AddRevisionVersion

	`AddVersion.exe $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini r 1`

4. DecRevisionVersion

	`AddVersion.exe $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini r 0`

5. Auto increment build version

	In visual studio, go through Project -- property -- build event -- after build -- command line:

	`$(ProjectDir)AfterBuild.bat $(ProjectDir) $(TargetDir)`
	
	This script will automaticall increment build version everytime you successfully build project.