var ffi = require("ffi")

var libwiringPi = ffi.Library('/usr/local/lib/libwiringPi', {
                              'wiringPiSetup' : [ 'int', [] ],
                              'digitalWrite' : [ 'void', ['int', 'int'] ],
                              'pinMode': [ 'void', ['int', 'int'] ],
                              'delayMicroseconds' :  [ 'void', ['int', 'int'] ]
                              })

var libLivolo = ffi.Library('plugins/livolo/libs/libLivoloWrapper', {
                              'newLivolo' : ['pointer', ['char'] ],
                              'Livolo_SendButton': ['void', [ 'pointer', 'int', 'char' ] ]
                              })

if (process.argv.length > 2) {
    if (process.argv.length < 5) {
        console.log('Arguments: Livolo Switch Code')
        process.exit()
    }

    var PIN = parseInt(process.argv[2]);
    var remoteId = parseInt(process.argv[3]);
    var keyCode = parseInt(process.argv[4]);
    var mySwitch = libLivolo.newLivolo(PIN);

    console.log("PIN: " + PIN);
    console.log("remoteId: " + remoteId);
    console.log("keyCode: " + keyCode);

    if (libwiringPi.wiringPiSetup() == -1){
        return 1;
        printf("Error initialising WiringPi");
    }
    
    if (mySwitch.isNull()) {
        console.log("Oh no! Couldn't create object!\n");
    } else {
        libLivolo.Livolo_SendButton(mySwitch, remoteId, keyCode);
    }
}

/**
 * Livolo on/off toggles
 */
function registerLivoloToggles() {
    $('button.gpio.toggle').unbind('click.toggle');
    $('button.gpio.toggle').bind('click.toggle', function()  {
                                 var button = $(this);
                                 button.parent().children('input').attr('value', button.attr('data-value'));
                                 });
}

require([ "jquery", "/socket.io/socket.io.js" ], function() {
        
        var socket = io.connect();
        
        registerGpioToggles();
        
        /**
         * GPIO input change
         */
        socket.on('livolo-input', function(data) {
                  $('.gpiostatus[data-id="' + data.id + '"]').addClass('hide');
                  $('.gpiostatus[data-id="' + data.id + '"][data-value="' + data.value + '"]').removeClass('hide');
                  });
        
        /**
         * GPIO output change
         */
        socket.on('livolo-output', function(data) {
                  $('button[data-id="' + data.id + '"]').removeClass('active');
                  $('button[data-id="' + data.id + '"][data-value="' + data.value + '"]').addClass('active');
                  });
        });