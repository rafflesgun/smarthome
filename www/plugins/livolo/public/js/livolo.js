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