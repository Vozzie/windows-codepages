
var fs = require('fs');
var filename = process.argv[2];
var name = filename.substr(filename.lastIndexOf('\\') + 1); name = name.substr(0, name.lastIndexOf('.'));
var uselead = !!(process.argv[3] || false);

function formatCodePoint(cp)
{
	cp = cp.toString(16).toUpperCase();
	return '0x0000'.substr(0, 6 - cp.length) + cp;
}

function createNoLead(data)
{
	var codepoints = new Array(0x100);
	for(var i = 0; i < codepoints.length; i++) codepoints[i] = 0xFFFF;
	for(var i = 0; i < data.length; i++)
	{
		var offset = data[i].indexOf('0x');
		if(offset > -1)
		{
			data[i] = data[i].substr(offset + 2);
			source = parseInt(data[i], 16);
			offset = data[i].indexOf('0x');
			if(offset > -1)
			{
				var target = parseInt(data[i].substr(offset + 2), 16);
				codepoints[source] = target;
			}
		}
	}
	for(var i = 0; i < codepoints.length; i++)
		codepoints[i] = formatCodePoint(codepoints[i]);
	console.log("static UTF16Char " + name + "_UNICODE[] = {" + codepoints.join(', ') + "};");
}

function createWithLead(data)
{
	var codepoints = new Array(0x10000);
	for(var i = 0; i < codepoints.length; i++) codepoints[i] = 0xFFFF;
	var leadbytes = new Array(0x100);
	for(var i = 0; i < leadbytes.length; i++) leadbytes[i] = 0;
	for(var i = 0; i < data.length; i++)
	{
		var offset = data[i].indexOf('0x');
		if(offset > -1)
		{
			data[i] = data[i].substr(offset + 2);
			source = parseInt(data[i], 16);
			offset = data[i].indexOf('#DBCS LEAD BYTE');
			if(offset > -1)
			{
				leadbytes[source] = 1;
			}
			else
			{
				offset = data[i].indexOf('0x');
				if(offset > -1)
				{
					var target = parseInt(data[i].substr(offset + 2), 16);
					codepoints[source] = target;
				}
			}
		}
	}
	console.log("static ANSIChar " + name + "_LEADBYTES[] = {" + leadbytes.join(', ') + "};");
	for(var i = 0; i < codepoints.length; i++)
		codepoints[i] = formatCodePoint(codepoints[i]);
	console.log("static UTF16Char " + name + "_UNICODE[] = {");
	var count = 0;
	while(codepoints.length > 0)
	{
		var row = codepoints.splice(0, 256).join(', ');
		if(codepoints.length == 0) console.log('\t' + row);
		else console.log('\t' + row + ', ')
	}
	console.log("};");
}

fs.readFile(process.argv[2], { encoding: 'ASCII'}, function (err, data) {
	if (err) throw err;
	data  = data.replace(/\r\n/gm,'\n').split('\n');
	if(uselead) createWithLead(data);
	else createNoLead(data);
});