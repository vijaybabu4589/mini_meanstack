var http = require('http');
var url = require('url');
//vijay
//function onrequest(req,res){
//	var q = ur	l.parse(req.url, true);
//	var qdata = q.query;
//	console.log(JSON.stringify(qdata.data));
//	console.log(String(qdata.dataa));
//res.end();
//}
//http.createServer(onrequest).listen('80','192.168.0.100');
//console.log('server started ...');


const express = require('express');
const app= express();

const bodyparser=require('body-parser');
app.use(bodyparser.urlencoded({extended : true}))

const mangoclient=require('mongodb').MongoClient
var db;

mangoclient.connect('mongodb://127.0.0.1:27017',(err,client)=>{

if(err) return console.log(err)

db=client.db('mydata');

app.listen(80,function(){
console.log('server started');


	app.get('/',function(req,res){
	res.send('home page');
	})


	app.get('/showdata',(req,res)=>{
	console.log(__dirname);

	var cursor=db.collection('myname').find().toArray(function(err,results){//connect to db
	if(err) return console.log(err)//display if any error

	//var jsondata=JSON.stringify(results); json to string
	//var jsondata=JSON.parse(results[0]);

	console.log(results[0]._id);//display id field value
	
	
	})
	res.sendFile(__dirname+'/index.html');
	})


	app.post('/adddata',(req,res)=>{
	
	var usertimestamp="volume."+datetime.getFullYear()+"."+(datetime.getMonth()+1);
	//console.log(typeof(usertimestamp));


	var qstring={};
	qstring[usertimestamp]=1;
	var qstring1={$inc:qstring};//same as {$inc:{volume.2018.11:1}}

	db.collection('myname').updateOne({_id:parseInt(req.body.value,10)},qstring1,(err,result)=>{
	if (err) return console.log(err)
	console.log(req.body.value);
	res.end();

	})

	res.end();
	})


})

})

