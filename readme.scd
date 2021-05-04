// ======= installation =======

in SuperCollider run:

(
// 1. create folder
if(File.exists(Platform.userExtensionDir).not, {
	File.mkdir(Platform.userExtensionDir);
});
)

// 2. open folder
Platform.userExtensionDir.openOS;

// 3. place the downloaded folder/files there

// 4. recompile class library


// ======= building from source =======

mkdir build
cd build
cmake -DSC_PATH=/Users/yota/supercollider/ ..
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
// cmake -DSUPERNOVA=ON ..
make