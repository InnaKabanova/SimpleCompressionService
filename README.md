# SimpleCompressionService
It consumes ASCII strings over a TCP socket and sends back compressed versions of
those strings. The target platform is Linux. 

## Table of contents
1. [Request format](#request_format)
2. [Response format](#response_format)
3. [Compression algorithm](#compr_algorithm)
4. [Building the components](#building_components)
5. [Implementation considerations](#impl_considerations)
6. [QtCreator project configuration](#qtcreator_tips)

## Request format <a name="request_format"></a>
Request = header + optional payload.

Request's header format (all fields are in network byte order):

* 32-bit magic value: 0x53545259.
* 16-bit payload length.
* 16-bit request code (see possible values below).

Request codes:
* Code: 1 ("Ping"). Checks if a service is operating normally.
* Code: 2 ("Get Stats"). Requests internal statistics of a service. Note: the statistics is individual for each service instance and does not persist through sessions. 
* Code: 3 ("Reset Stats"). Resets internal statistics of a service.
* Code: 4 ("Compress"). Requests some data to be compressed (compression algorithm is described below).

Any other request codes are considered invalid.

Requests of types "Ping", "Get Stats", "Reset Stats" consist of only a header, where the payload length field is set to 0. A "Compress" request consists of a header and ASCII payload intended for compression. Maximum payload size is TBD: any payload that is larger results in an appropriate error (see response formats below).      

## Response format <a name="response_format"></a>
For each request, there is an appropriate response to be sent in return. Response = header + optional payload.

Response's header format (all fields are in network byte order):

* 32-bit magic value: 0x53545259.
* 16-bit payload length.
* 16-bit response code (see possible values below).

Response codes:
* Code: 0 ("OK").
* Code: 1 ("Uknown Error").
* Code: 2 ("Payload Too Large").
* Code: 3 ("Unsupported Request Type").
* (...) TBD

### "Ping" response details
"Ping" response = header with payload length field set to 0 and status code set to: 

* 0 ("OK") -- the service is operating normally.
* code of an error if one occured.

### "Get Stats" response details
TODO

### "Reset Stats" response details
TODO

### "Compress" response details
TODO

## Compression algorithm <a name="compr_algorithm"></a>
TODO

## Building the components <a name="building_components"></a>
Run the 'build.sh' script which is located in the root of the project:

* './build.sh 1' (or './build.sh s') -- build the 'simple_compr_service' binary
* './build.sh 2' (or './build.sh t') -- build the 'test_client' binary
* './build.sh 3' (or './build.sh l') -- build the 'lib_common' shared library
* './build.sh 4' (or './build.sh b') -- build all the stuff above
* './build.sh 5' (or './build.sh c') -- clean all the built stuff above

## Implementation considerations <a name="impl_considerations"></a>
TODO

## QtCreator project configuration <a name="qtcreator_tips"></a>
Below I left a quick reminder for myself on how to build/run simple_compression_service and test_client binaries from QtCreator, which is my C/C++ IDE of choice. 

![Build SCS](https://raw.githubusercontent.com/InnaKabanova/SimpleCompressionService/master/qtcreator_tips/scs_build_qtcreator.png)
![Run SCS](https://raw.githubusercontent.com/InnaKabanova/SimpleCompressionService/master/qtcreator_tips/scs_run_qtcreator.png)
![Build TC](https://raw.githubusercontent.com/InnaKabanova/SimpleCompressionService/master/qtcreator_tips/tc_build_qtcreator.png)
![Run TC](https://raw.githubusercontent.com/InnaKabanova/SimpleCompressionService/master/qtcreator_tips/tc_run_qtcreator.png)

If simple_compression_service daemon is already running, it can be attached to with gdb from QtCreator as well: **Debug > Start Debugging > Attach to Running Application...** If you can't attach because of ptrace, the possible solution is described here: https://stackoverflow.com/questions/22120711/qt-creator-ptrace-operation-not-permitted-what-is-the-permanent-solution.  











