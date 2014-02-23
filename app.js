// Module Dependencies
var express = require('express'),
  http = require('http'),
  path = require('path');
var exec = require('child_process').execFile,
    child;

var app = express();

var routes = require('./routes');

app.configure(function () {
  app.set('port', process.env.PORT || 3000);
  app.use(express.favicon());
  app.use(express.logger('dev'));
  app.use(express.bodyParser());
  app.use(express.methodOverride());
  app.use(app.router);
  app.use(express.static(path.join(__dirname, 'public')));
});

app.configure('development', function () {
  app.use(express.errorHandler());
});

app.get('/', function (req,res) {
  res.sendfile('public/index.html');
});
app.get('/create', function (req,res) {
  res.sendfile('public/create.html');
});

var server = http.createServer(app).listen(app.get('port'), function () {
  console.log('Express server listening on port ' + app.get('port'));
});

var io = require('socket.io').listen(server);

io.sockets.on('connection', function (client) {
  console.log('client connected: '+client);

  client.on('record', function (data) {
    var cmd = 'rpi/bash/ir_hub.sh record ' + data;
    console.log('Executing Command: '+cmd);
    child = exec('rpi/bash/ir_hub.sh', ['record', data],// command line argument directly in string
      function (error, stdout, stderr) {      // one easy function to capture data/errors
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (error !== null) {
          console.log('exec error: ' + error);
        }
    });
  });
});
