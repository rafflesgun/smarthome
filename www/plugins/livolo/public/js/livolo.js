/**
 * Livolo on/off toggles
 */
function registerLivoloToggles() {
    $('button.livolo.toggle').unbind('click.toggle');
    $('button.livolo.toggle').bind('click.toggle', function()  {
                                 var button = $(this);
                                 button.parent().children('input').attr('value', button.attr('data-value'));
                                 });
}

require([ "jquery", "/socket.io/socket.io.js" ], function() {
        
        var socket = io.connect();
        
        registerLivoloToggles();

        /**
         * Livolo change
         */
        socket.on('livolo-output', function(data) {
                  $('button[data-id="' + data.id + '"]').removeClass('hide');
                  $('button[data-id="' + data.id + '"][data-value="' + data.value + '"]').addClass('hide');
                  });
        });
