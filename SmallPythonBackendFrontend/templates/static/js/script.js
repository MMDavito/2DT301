$( document ).ready(function() {

   function clearTextField() {
     //var inputTextArea = document.getElementById("inputTextArea").value="";
     var inputTextArea = document.getElementById("inputTextArea");
     console.log(inputTextArea.value);
     inputTextArea.value = "";
   }
 
   function getData() {
     var credentials = document.getElementById("credentials").value;
     var inputTextArea = document.getElementById("inputTextArea").value;

     console.log("You fucker Credentials: "+credentials)
     console.log("You fucker adding data: "+inputTextArea)
     //var data = imageData.data;
     
     $.post( "/postmethod", {
       ///"canvas_data": JSON.stringify(outputData)
       "Credentials":credentials,
       "data":inputTextArea
     }, function(resp){
       console.log("SUNOBITCH");
       //console.log("CONTENT: "+JSON.stringify(resp));
       //resp = JSON.stringify(resp);
       console.log("CONTENT: "+resp["filename"]);

       window.location.href = "/results/"+resp["filename"];  
     });
   }
 
   $( "#clearButton" ).click(function(){
     clearTextField();
   });
 
   $( "#sendButton" ).click(function(){
     getData();
   });
 });