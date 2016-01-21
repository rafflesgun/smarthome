var ffi = require("ffi")

var libwiringPi = ffi.Library('/usr/local/lib/libwiringPi', {
                              'wiringPiSetup' : [ 'int', [] ],
                              'digitalWrite' : [ 'void', ['int', 'int'] ],
                              'pinMode': [ 'void', ['int', 'int'] ],
                              'delayMicroseconds' :  [ 'void', ['int', 'int'] ]
                              })

var libLivolo = ffi.Library('./libLivoloWrapper', {
                              'newLivolo' : ['pointer', ['char'] ],
                              'Livolo_SendButton': ['void', [ 'pointer', 'int', 'char' ] ]
                              })

var pinMapping = {
    "3": 8,
    "5": 9,
    "7": 7,
    "8": 15,
    "10": 16,
    "11": 0,
    "12": 1,
    "13": 2,
    "15": 3,
    "16": 4,
    "18": 5,
    "19": 12,
    "21": 13,
    "22": 6,
    "23": 14,
    "24": 10,
    "26": 11,
    "29": 21,
    "31": 22,
    "32": 26,
    "33": 23,
    "35": 24,
    "36": 27,
    "37": 25,
    "38": 28,
    "40": 29
};

function isNumber(number) {
    return !isNaN(parseInt(number, 10));
}

function sanitizePinNumber(pinNumber) {
    if (!isNumber(pinNumber) || !isNumber(pinMapping[pinNumber])) {
        throw new Error("Pin number isn't valid");
    }
    
    return parseInt(pinNumber, 10);
}

var livolo = {

mySwitch,
    
open: function(pinNumber, callback) {
    pinNumber = sanitizePinNumber(pinNumber);
    
    mySwitch = libLivolo.newLivolo(pinNumber);
    
    if (libwiringPi.wiringPiSetup() == -1){
        throw new Error("Error initialising WiringPi");
    }
    
    if (mySwitch.isNull()) {
        throw new Error("Error initialising Livolo");
    }
},

    
close: function(pinNumber, callback) {
    pinNumber = sanitizePinNumber(pinNumber);
    
    libLivolo.delete(mySwitch);
},
    
read: function(groupId, deviceId, callback) {
    groupId = sanitizePinNumber(groupId);
    deviceId = sanitizePinNumber(deviceId);
},
    
write: function(groupId, deviceId, callback) {
    groupId = sanitizePinNumber(groupId);
    deviceId = sanitizePinNumber(deviceId);
    
    libLivolo.Livolo_SendButton(mySwitch, groupId, deviceId);
}
};

livolo.export = livolo.open;
livolo.unexport = livolo.close;

module.exports = livolo;