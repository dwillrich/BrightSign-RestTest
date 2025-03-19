# Fake Rest Test

## Overview
Simple rest application to fetch JSON data from url and parse it to return various calculations

## Building / Running
Building and developed on Ubuntu so mileage may vary with other distros.

To build ensure you have ensure you have all required packages installed. See Dockerfile for the full list if you wish to build natively. Simply run the following to build.
```
./build.sh
```

To build a docker image and build inside docker run the following. This will ensure a sane build environment with all necesary packages. Note: nothing here is version locked but in a production enviorment it would be wise to do so for reproducable build artifacts.
```
./build-docker.sh
```

For running the application simply call it with the target URL or it will default to the following "http://test.brightsign.io:3000".
```
./build/fakerest
```

For running the test simply call the *-tests bin generated in the build output directory.
```
./build/fakerest-test
```

## REST Data Format
```
Insert a sample returned entry here
```

## Output


## System Requirements & Assumptions
TODO - Talk about memory footprint required for this
TODO - Talk about use of JSONCpp or other JSON lib
TODO - Optional

