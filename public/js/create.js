var socket = io.connect();

var count = 0;
var saveArr = [];

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
								+'<td><button type="button" id="'+(count++)+'"class="select btn btn-info">Select</button></td>'
								+'</tr>'
			);
		$('#'+(count-1)).click(function () {
			var val = $(this).attr('id');
			$('.preview').append('<li data-preview="'+(count-1)+'">'+name+' -- '+hex+'</li>');
			saveArr.push(arr);
		});
	});
	$('#reset').click(function () {
		$('.preview').html('');
		saveArr = [];
	});
	$('#save').click(function () {
		if(saveArr.length < 1){
			alert('Nothing selected!');
		}
		else if(($('#macroname').val()) && ($('#macrodesc').val())){
			socket.emit('saveMacro', {'name':$('#macroname').val(), 'desc':$('#macrodesc').val(), 'data':saveArr});
		}
		else{
			alert('Please make sure you fill in name and description');
		}
	});
});