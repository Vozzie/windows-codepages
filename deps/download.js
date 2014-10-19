
var http = require('http');
var fs = require('fs');


function getOptions()
{
	return  {
		host: 'www.unicode.org',
		path: '/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/' + (arguments[0] || '')
	};
}

var callbackFile = function(response) {
	var data = [];
	response.on('data', function (chunk) { data.push(chunk); });
	response.on('end', function () {
		var filename = response.req.path;
		filename = filename.substr(filename.lastIndexOf('/') + 1);
		data = data.join('');
		fs.writeFile(filename, data);
	});
};


var callbackFolder = function(response) {
	var data = [];
	response.on('data', function (chunk) { data.push(chunk); });

	response.on('end', function () {

		data = data.join('');
		var re = /href="([\w\.]+)"/g;
		var m;
		
		while((m = re.exec(data)) != null)
		{
			if(m.index === re.lastIndex) re.lastIndex++;
			console.log("Download: " + m[1]);
			http.request(getOptions(m[1]), callbackFile).end();
		}
	});
};

http.request(getOptions(), callbackFolder).end();

