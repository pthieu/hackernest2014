var socket = io.connect();

$( document ).ready(function() {
	$('#record').click(function () {
		socket.emit('record', $('#inputRecord').val());
	});
});