$( document ).ready(function() {

   function clearTextField() {
     //var inputTextArea = document.getElementById("inputTextArea").value="";
     var inputTextArea = document.getElementById("inputTextArea");
     console.log(inputTextArea.value);
     inputTextArea.value = "";
   }
 
   function getData() {
     var inputTextArea = document.getElementById("inputTextArea");
     console.log("You fucker adding data: "+inputTextArea.value)
     //var data = imageData.data;
     
     $.post( "/postmethod", {
       ///"canvas_data": JSON.stringify(outputData)
       "credentials":"BAJS",
       "data":"DATABAJS"
     }, function(err, req, resp){
       console.log("SUNOBITCH")
       window.location.href = "/results/"+resp["responseJSON"]["uuid"];  
     });
   }
 
   $( "#clearButton" ).click(function(){
     clearTextField();
   });
 
   $( "#sendButton" ).click(function(){
     getData();
   });
 });