var socket = io.connect();
var count  = 0;

$( document ).ready(function() {
	socket.emit('getIRCodes');
	socket.on('setIRCodes', function (data) {
		var name = data[0];
		var hex = data[2];
		var arr = [data[0], data[1], data[2], data[3]];
		var tmp = $('.cmdslist').append('<tr data-code="'+count+'"><td>'+data[0]+'</td>'
								+'<td>'+data[1]+'</td>'
								+'<td>'+data[2]+'</td>'
								+'<td>'+data[3]+'</td>'
								+'<td><button type="button" id="'+(count++)+'"class="select btn btn-info" data-code="'+name+'">Run Macro</button></td>'
								+'</tr>'
			);
		$('#'+(count-1)).click(function () {
			var val = $(this).attr('data-code');
			socket.emit('runMacro', val);
		});
	});

	$('#record').click(function () {
		socket.emit('record', $('#inputRecord').val());
	});
	$('#run').click(function() {
		
	});
});