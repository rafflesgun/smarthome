var ffi = require("ffi")

var libwiringPi = ffi.Library('/usr/local/lib/libwiringPi', {
                              'wiringPiSetup' : [ 'int', [] ],
                              'digitalWrite' : [ 'void', ['int', 'int'] ],
                              'pinMode': [ 'void', ['int', 'int'] ],
                              'delayMicroseconds' :  [ 'void', ['int', 'int'] ]
                              })

var libLivolo = ffi.Library('./node_modules/pi-livolo/libLivoloWrapper', {
                              'newLivolo' : ['pointer', ['char'] ],
                              'Livolo_SendButton': ['void', [ 'pointer', 'int', 'char' ] ]
                              })

if (process.argv.length < 5) {
    console.log('Arguments: Livolo Switch Code')
    process.exit()
}

var PIN = parseInt(process.argv[2]);
var remoteId = parseInt(process.argv[3]);
var keyCode = parseInt(process.argv[4]);
var mySwitch = libLivolo.newLivolo(PIN);

if (libwiringPi.wiringPiSetup() == -1){
    return 1;
    printf("Error initialising WiringPi");
}

if (mySwitch.isNull()) {
    console.log("Oh no! Couldn't create object!\n");
} else {
    libLivolo.Livolo_SendButton(mySwitch, remoteId, keyCode);
}