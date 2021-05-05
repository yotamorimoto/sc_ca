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


// ======= build from source =======

mkdir build
cd build
cmake -DSC_PATH=/Users/yota/supercollider/ -DSUPERNOVA=ON ..
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make
