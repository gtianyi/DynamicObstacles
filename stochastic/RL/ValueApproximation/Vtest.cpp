var http = require('http');
var fs = require('fs');
var words = fs.readFileSync('words.json');
var data = JSON.parse(words);


function onRequest(request, response) {
	response.writeHead(200, {'Content-Type': 'text/html'});
	fs.readFile('./formnjs.html', null, function(error, data) {
		if (error){
			response.writeHead(404);
			response.write('HTTP 404 Page Not Found');
		} else{
			response.write(data);
					}
		response.end();
	});
	}

http.createServer(onRequest).listen(8000);
console.log("Server is Listening.......")