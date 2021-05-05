# Hacking Cellular Automata

![rulemix]
(https://octodex.github.com/images/yaktocat.png)

## installation

1. in SuperCollider run:

```
if(File.exists(Platform.userExtensionDir).not, {File.mkdir(Platform.userExtensionDir)});
```
2. open folder
`Platform.userExtensionDir.openOS;`

3. place the downloaded folder/files there

4. recompile class library


## build from source
```
mkdir build
cd build
cmake -DSC_PATH=/Users/yota/supercollider/ -DSUPERNOVA=ON ..
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make
```

# reference
https://github.com/yotamorimoto/sc_ca/blob/main/hacking.pdf
