# SimpleCompressionService
It consumes ASCII strings over a TCP socket and sends back compressed versions of
those strings. The target platform is Linux. 

## Request format
TODO

## Response format
TODO

## Compression algorithm
TODO

## Building the components
Run the 'build.sh' script which is located in the root of the project:

* './build.sh 1' (or './build.sh s') -- build the 'simple_compr_service' binary
* './build.sh 2' (or './build.sh t') -- build the 'test_client' binary
* './build.sh 3' (or './build.sh l') -- build the 'lib_common' shared library
* './build.sh 4' (or './build.sh b') -- build all the stuff above
* './build.sh 5' (or './build.sh c') -- clean all the built stuff above

## Implementation considerations
TODO
