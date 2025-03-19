# Fake Rest Test

## Overview
Simple rest application to fetch JSON data from url and parse it to return various calculations. As it is just being built natively for x86 no cross compilation considerations were made for the build environment but could easily be added with CMake.  

## Source Breakdown
### src/Utils
Directory where all of the support classes are defined and build into a lib for use with the tests or in the main application. As per the requirements and thinking with a "unix philosophy" these Utils could become part of a core library for a system and be expanded as new abilities are needed. For simplicity they are linked privately without deploying include files but could be seperated out to be a shared lib for the system.
#### src/Utils/Logger.*
A quick logging implementation for the application to easily switch where debug and run time information is sent. During development it was easiest to just leave it as stdout but with the requirements stating "The data output must be in JSON format suitable for analysis with a CLI tool like jq." this was interpreted as being able to directly pipe the output to `jq` and thus the debug output was switched to log to busybox syslog and can be monitored with `logread`.
#### src/Utils/SimpleUrlFetcher.*
Small class to wrap curl calls to a given URL. This in a more general use case could be expanded to make REST calls with proper security measures in place. See security section below.  
#### src/Utils/UrlValidator.*
A very small class with a simple regex to check if a URL is valid. Mainly used just as an example of a testable class and to push forward some of the boilerplate tooling required for the repository. 

## Building / Running
Building and developed on Ubuntu so mileage may vary with other distros.

To build ensure you have ensure you have all required packages installed. See Dockerfile for the full list if you wish to build natively. Simply run the following to build. Note: This will do a full rebuild each time to simplify things as it is such a small repository. 
```bash
./build.sh
```

To build a docker image and build inside docker run the following. This will ensure a sane build environment with all necesary packages. Note: nothing here is version locked but in a production enviorment it would be wise to do so for reproducable build artifacts.
```bash
./build-docker.sh
```

For running the application simply call it with the target URL or it will default to the following "http://test.brightsign.io:3000".
```bash
./build/fakerest
```

For running the test simply call the *-tests bin generated in the build output directory.
```bash
./build/fakerest-test
```

## REST Data Format
```json
Insert a sample returned entry here
```

## Output
```json
Insert a sample response here
```

## Security Considerations
TODO - Add security notes here on access tokens OAuth SSL etc.

## Error Handling
TODO - Note on the error handling implemented



## System Requirements & Assumptions
TODO - Talk about memory footprint required for this
TODO - Talk about use of JSONCpp or other JSON lib
TODO - Optional

