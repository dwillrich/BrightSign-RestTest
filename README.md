# Fake Rest Test

## Overview
Simple rest application to fetch JSON data from url and parse it to return various calculations. As it is just being built natively for x86 no cross compilation considerations were made for the build environment but could easily be added with CMake. Optionally can be built in Docker image.

## Source Breakdown
### src/Utils
Directory where all of the support classes are defined and build into a lib for use with the tests or in the main application. As per the requirements and thinking with a "unix philosophy" these Utils could become part of a core library for a system and be expanded as new abilities are needed. For simplicity they are linked privately without deploying include files but could be seperated out to be a shared lib for the system.
#### src/Utils/Logger.*
A quick logging implementation for the application to easily switch where debug and run time information is sent. During development it was easiest to just leave it as stdout but with the requirements stating "The data output must be in JSON format suitable for analysis with a CLI tool like jq." this was interpreted as being able to directly pipe the output to `jq` and thus the debug output was switched to log to busybox syslog and can be monitored with `logread`.
#### src/Utils/SimpleUrlFetcher.*
Small class to wrap curl calls to a given URL. This in a more general use case could be expanded to make REST calls with proper security measures in place. See security section below.  
#### src/Utils/UrlValidator.*
A very small class with a simple regex to check if a URL is valid. Mainly used just as an example of a testable class and to push forward some of the boilerplate tooling required for the repository. 
#### src/Utils/UsersAndFriends.*
Class for parsing JSON into User C++ objects and ability to log classes if desired (unused). Can convert a string with json or a file containing json data to a vecotr of Users. 

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

## Logging
To view logs they are defaulted writing to syslog. You can change this with the defines at the top of Logger.cpp. To view logs while it runs use the following command:
```bash
tail -f /var/log/syslog
```

## REST Data Format
```json
{
  "id": 800017,
  "name": "Amelia",
  "city": "Charleston",
  "age": 78,
  "friends": [
    {
      "name": "Grace",
      "hobbies": [
        "Church Activities",
        "Collecting",
        "Dancing"
      ]
    },
    {
      "name": "Camila",
      "hobbies": [
        "Collecting",
        "Martial Arts",
        "Socializing"
      ]
    }
  ]
}

```
We will call the top level JSON a User for the purpose of this application and call each friends entry a Friend. And denote fields in this document in C style ie. `user.name`
Notes on User fields:
`user.id` - This is a `uint` for a unique id for the user. Is not always present though. Not sure what policy should be on this.
`user.name` - This is a `string` representing the name of the user. These are not unique it seems.
`user.city` - This is a `string` representing the city of the user. 
`user.age` - This is a `uint` representing the age of the user
`user.friends` - This is an array of Friends

`friend.name` - Seems to always be present but non-unique
`friend.hobbies` - Seems to always be present? Can it be empty or missing?

General Notes:
- Will respond with HTML error page, 500 error on page, no data or actual JSON
- Sometimes is sent as an array [] -> ruins streaming the url line by line
- Sometimes is sent as a json object per line
- Sometimes is sent as a pretty printed non-array set of json objects
- Sometimes is missing fields id and city
- Assume some people have no friends
- Some people have no hobbies?
- Will respond with HTML error page, 500 error on page, no data or actual JSON

## Output
```json
{
  "C1": {
    "Austin": 58.558801591248134,
    "Boston": 57.863036303630366
  },
  "C2": {
    "Austin": 4.006464445549478,
    "Boston": 4.006854531607007
  },
  "C3": {
    "Austin": {
      "age": 42,
      "city": "Austin",
      "friends": [
        {
          "hobbies": [
            "Volunteer Work",
            "Gardening",
            "Cooking"
          ],
          "name": "Levi"
        },
        {
          "hobbies": [
            "Podcasts",
            "Traveling",
            "Woodworking"
          ],
          "name": "Ava"
        },
        {
          "hobbies": [
            "Martial Arts",
            "Traveling",
            "Housework"
          ],
          "name": "Noah"
        },
        {
          "hobbies": [
            "Yoga",
            "Quilting",
            "Housework"
          ],
          "name": "Daniel"
        },
        {
          "hobbies": [
            "Housework",
            "Writing"
          ],
          "name": "Sarah"
        },
        {
          "hobbies": [
            "Yoga",
            "Martial Arts",
            "Church Activities"
          ],
          "name": "Sophie"
        }
      ],
      "id": 400097,
      "name": "Michael"
    },
    "Boston": {
      "age": 79,
      "city": "Boston",
      "friends": [
        {
          "hobbies": [
            "Socializing",
            "Painting"
          ],
          "name": "Mateo"
        },
        {
          "hobbies": [
            "Podcasts",
            "Shopping"
          ],
          "name": "Robert"
        },
        {
          "hobbies": [
            "Walking",
            "Dancing"
          ],
          "name": "Oliver"
        },
        {
          "hobbies": [
            "Cooking",
            "Genealogy"
          ],
          "name": "Elijah"
        },
        {
          "hobbies": [
            "Walking",
            "Dancing",
            "Genealogy"
          ],
          "name": "Kevin"
        },
        {
          "hobbies": [
            "Cooking",
            "Dancing",
            "Television"
          ],
          "name": "Luke"
        }
      ],
      "id": 400119,
      "name": "Mateo"
    }
  },
  "C4": "Mia",
  "C5": "Woodworking"
}
```
Output of the program is just the json with all calculations in it using the format C# denoting the calculation. For C1-C3 the data is output as a map of city->value and for C4 and C5 it is just output as a string. To parse out just one calculation pipe it to jq. ie. For C4 use this `./build/fakerest | jq .C4` 


## Security Considerations
Ideally this API would have a valid SSL certificate that we could verify and we could use the following curl options to verify it.
```c
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
```
If this was actual production userdata we would want to secure the data with some form of encryption such as TLS and thus use HTTPS.
If this was actual production userdata we would want to add some sort of authentication to the endpoint as well and not leave it open. Likely use some sort of JWT or OAth token passing from the client. 

## Error Handling
The general error handling philosophy used in the application varies based on function. For the data fetching all error cases are pushed up to the caller with invalid/null response data. For the parsing of Users data the error handling is a bit more forgiving to allow for the poor API. When JSON syntax errors occur we will process all data up to that error and log the error (see BUG-4), parsing will return false but the data to that point will still be returned for caller to decide what to do. The top level will return 0 with the json analytics printed if everything is processed correctly and return 1 in all error cases. 

## System Requirements & Assumptions
### Memory
This implementation expects sufficient RAM to hold the returned data for the REST endpoint and enough RAM to hold an additional copy of the data as c++ classes. If RAM consumption is an issue the REST response could be written to file and manipulated there but for simplicity sake it is handled in RAM. The SimpleUrlFetcher is provisioned to handle writing to file and the parsing also can accept a file as input. These were useful in testing but could be used in a production client. Additionally the SimpleUrlFetcher can do processing line by line to avoid the full response and C structs being in RAM at the same time, this would ideally be used if data is sent one User object per line. Data is passed by const reference or referance where ever possible and as for the Analytics it is done with shared pointers to avoid copy operations. 

### Execution Speed
- The API is given 5 seconds to respond. This is arbitrary but I tried to test with throttled network and it seemed sufficient. 
- Notes on the analytics performance can be seen in the comments in UserAnalytics.cpp
- Threading could be used for the data processing to achieve better throughput but in theory it may be better to have the calling application specify which result it wants and run each analytic task on its own thread with the memory consuption increasing being the tradeoff here. 
- getCityUserMap in Analytics shouldn't need to be run for each function and could be run once

## Improvements
- Logger would be nice to make printf compatible with vargs and not just accept strings.
- Evaluate performance further in terms of speed and memory usage.
- For whatever reason the json.hpp header isn't found so it is coppied into src/ dir for now despite being looked for by CMAKE.


## API Issues
In summary this API is far from ideal and is not production ready. Building out the client there are a number of issues seen that will be listed here as issue tickets `BUG-<num>`. This is not an all inclusive list as I am sure there are some overlooked here. 

#### BUG-1 : [API][Major] API Does not properly HTTP return error codes
- As see with 400 error code as well as 500 the curl of the API will return a HTML page stating the error but still return 200

#### BUG-2 : [API][Major] Inconsistent API return data format
- The list of users returned is not consistently formatted. It is sent in one of three ways from testing : a JSON array of Users, a concatenated set of users (non-comma seperated or in square brackets), and a list of users with one JSON object per line. 

#### BUG-3 : [API][Minor] Trailing whitespace at end of data
- The API will return with trailing whitespace at end of data. This is more of an annoyance than a bug. 

#### BUG-3 : [API][Major] Users occationally missing fields.
- The API will return Users that do not have all fields populated. This has been commonly seen with `id` field but also seen with `age` as well
- For now we ignore missing `id` in the client and users with no `age` are skipped when age analytics are being done.

#### BUG-4 : [API][Major] JSON returned occasionally not parsable by `jq`
- This is where I drew a line in the sand on "nicely" handling the API responses. Occasionally even jq does not properly parse the responses containing JSON data returned.
- Would be easy enough to gather more data on this by modifying the getSampleResponses.sh tools script to pipe to jq and only save off the responses that fail that check.
- Quick glance and I see `]}]}{` at the end of a line occasionally and just stop processing data after that error occurs.  
