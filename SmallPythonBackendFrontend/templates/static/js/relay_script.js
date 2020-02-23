console.log("HURAY FOR ME!");

function yesnoCheck() {
    if (document.getElementById("noCheck").checked) {
        $(".div_start_time").hide();
    } else {
        $(".div_start_time").show();
    }
    /*
    var divStartTime = document.getElementsByClassName("div_start_time");
    console.log("Length?: "+divStartTime.length);
  
    for(var i = 0; i<divStartTime.length;i++){
      var element = divStartTime[i];
      if(document.getElementById("noCheck").checked){
        //element.style.display="none";
        element.style.visibility="hidden";
      }else{
        element.style.display="visible";
      }
    }*/

}
function printBajs() {
    console.log("BAJS");;
}
function clearSkit() {
    console.log("Will now clear");
    var data = sessionStorage.getItem("relays");
    //console.log("SHITTY DATA:\n"+data);
    data = JSON.parse(data);
    //console.log("SHITTiest DATA:\n"+JSON.stringify(data["data"]));
    var relays = data["data"];
    relays["relays"].forEach(relay => {
        document.getElementById("isOff" + relay["id"]).checked = true;
    })

    /*//FOllowing is just incase of future javascript effort.
    var divCont = document.getElementById("FormBajs");
    if(!divCont.hasChildNodes()){
      console.log("No children of div!");
      return;
    }
    //ELSE:
    var forms = divCont.children;
    forms.forEach(form =>{
      if(!form.hasChildNodes()) continue;
      //ELSE:
 
    });
    */
}
function sendSkit() {
    console.log("Will now SEND");
    var data = sessionStorage.getItem("relays");
    //console.log("SHITTY DATA:\n"+data);
    data = JSON.parse(data);
    //console.log("SHITTiest DATA:\n"+JSON.stringify(data["data"]));
    var relays = data["data"];
    var i = 0;
    relays["relays"].forEach(relay => {
        var isOn = false;
        //console.log("Does this work?: "+"isOn"+relay["id"]);
        //console.log("IS TEMPPPP?: "+temp);
        if (document.getElementById("isOn" + relay["id"]).checked) {
            isOn = true;
        }
        console.log("ID: " + relay["id"] + ", is on?: " + isOn);
        relay["relay_is_on"] = isOn;

        //relay["relay_is_on"] = document.getElementById("isON"+relay["id"]).checked;
    })
    console.log("Relays after:\n" + JSON.stringify(relays));
    $.ajax({
        url: "/arduino_relay",
        type: 'POST',
        dataType: 'json',
        data: JSON.stringify(relays),
        //headers:{"credentials":"BAJS"},
        //credentials:"BAJS",
        beforeSend: function (request) {
            request.setRequestHeader("x-api-key", 1337);
            request.setRequestHeader("Credentials", "BAJS");
        },

        contentType: 'application/json; charset=utf-8',
        success: function (result) {
            console.log("RESULTATFAN EFTER: \n" + result)
        },
        error: function (request, status, error) {
            console.log("request: " + request);
            console.log("status: " + status);
            console.log("ERROR: " + error);
        }
    });
}


$.ajax({
    url: "/arduino_relay",
    type: 'GET',
    dataType: 'json',
    //headers:{"credentials":"BAJS"},
    //credentials:"BAJS",
    beforeSend: function (request) {
        request.setRequestHeader("x-api-key", 1337);
        request.setRequestHeader("Credentials", "BAJS");
    },

    contentType: 'application/json; charset=utf-8',
    success: function (result) {
        sessionStorage.setItem("relays", JSON.stringify(result));
        console.log("RESULTATFAN: " + JSON.stringify(result));
        console.log("Är detta data?: " + result["data"]);
        var data = result["data"]["relays"];
        //sessionStorage.setItem("relays",result["data"]);

        var li = document.getElementById("listfan");
        var DivForControl = document.getElementById("DivForControl");
        var FormBajs = document.getElementById("FormBajs");
        var i = 0;
        data.forEach(element => {
            var textNode = document.createTextNode(JSON.stringify(element));
            var node = document.createElement("LI");
            node.appendChild(textNode);
            li.appendChild(node);

            //TODO: WIll try to make child of label id "ON" or "OFF".
            var formDiv = document.createElement("div");
            formDiv.id = "form_relay_" + element["id"];
            formDiv.classList.add("panel");
            formDiv.classList.add("panel-default");
            formDiv.classList.add("form-check");
            //formDiv.innerHTML("<h1>Relay: "+data["id"]+"</h1>");
            console.log("ID?: " + element["id"]);
            var innerText = document.createTextNode("Relay " + element["id"] + ": ");
            formDiv.appendChild(innerText);
            var lineBreak = document.createElement("br");
            formDiv.appendChild(lineBreak);

            var labelON = document.createElement("label");
            var radioON = document.createElement("input");
            labelON.classList.add("form-check-label");
            radioON.classList.add("form-check-input");

            radioON.type = "radio";
            radioON.name = "yes_no_" + element["id"];
            radioON.id = "isOn" + element["id"];
            radioON.value = "isOn";
            if (element["relay_is_on"]) console.log("Helvetes skit");
            //radioON.checked = element["relay_is_on"];
            //radioON.checked =true;

            labelON.appendChild(radioON);
            labelON.innerHTML += "ON";
            formDiv.appendChild(labelON);

            var labelOFF = document.createElement("label");
            var radioOFF = document.createElement("input");
            labelOFF.classList.add("form-check-label");
            radioOFF.classList.add("form-check-input");
            radioOFF.name = "yes_no_" + element["id"];
            radioOFF.type = "radio";
            radioOFF.id = "isOff" + element["id"];
            radioOFF.value = "isOff";
            labelOFF.appendChild(radioOFF);
            labelOFF.innerHTML += "OFF";
            formDiv.appendChild(labelOFF);
            var lineBreak = document.createElement("br");
            formDiv.appendChild(lineBreak);

            //Hours
            var label = document.createElement("label");
            var input = document.createElement("input");
            label.innerHTML += "Hours: ";
            label.classList.add("form-check-label");
            input.classList.add("form-check-input");
            input.id = "hours_" + element["id"];
            input.type = "number";
            input.name = "hours" + element["id"];
            input.min = "0";
            input.max = "10";
            label.appendChild(input);
            formDiv.appendChild(label);
            //Minutes
            label = document.createElement("label");
            input = document.createElement("input");
            label.innerHTML += " Minutes: ";
            label.classList.add("form-check-label");
            input.classList.add("form-check-input");
            input.id = "minutes_" + element["id"];
            input.type = "number";
            input.name = "minutes" + element["id"];
            input.min = "0";
            input.max = "60";
            label.appendChild(input);
            formDiv.appendChild(label);
            //Seconds
            label = document.createElement("label");
            input = document.createElement("input");
            label.innerHTML += " Seconds: ";
            label.classList.add("form-check-label");
            input.classList.add("form-check-input");
            input.id = "seconds_" + element["id"];
            input.type = "number";
            input.name = "seconds" + element["id"];
            input.min = "0";
            input.max = "60";
            label.appendChild(input);
            formDiv.appendChild(label);
            var lineBreak = document.createElement("br");

            formDiv.appendChild(lineBreak);


            label = document.createElement("label");
            input = document.createElement("input");
            label.classList.add("form-check-label");
            input.classList.add("form-check-input");
            label.innerHTML += "Start time: ";
            input.id = "start_time_" + element["id"];//null if start imidetly (willl)
            input.type = "time";
            input.step = "1";
            label.append(input);
            if (i != 0) {
                var divStartTime = document.createElement("div");
                //divStartTime.className="div_start_time";
                divStartTime.classList.add("div_start_time");
                //formDiv.appendChild(divStartTime);
                divStartTime.appendChild(label);
                formDiv.appendChild(divStartTime);
            }
            else {
                formDiv.appendChild(label);
            }

            label = document.createElement("label");
            input = document.createElement("input");
            label.classList.add("form-check-label");
            input.classList.add("form-check-input");
            label.innerHTML += " Number of repeats (-1 if infinite): ";

            input.id = "num_repeats" + element["id"];
            input.type = "number";
            input.name = "num_repeats" + element["id"];
            input.min = "-1";
            input.max = "10";
            label.appendChild(input);

            if (i == 0) {
                var lineBreak = document.createElement("br");
                formDiv.appendChild(lineBreak);
            }
            formDiv.appendChild(label);

            FormBajs.appendChild(formDiv);
            document.getElementById("isOn" + element["id"]).checked = element["relay_is_on"];
            document.getElementById("isOff" + element["id"]).checked = !element["relay_is_on"];
            //formDiv.appendChild(lineBreak);
            i++;
        });

        if (document.getElementById("noCheck").checked) {
            //divStartTime.style.display="none";
            //divStartTime.style.visibility="hidden";
            $(".div_start_time").hide();
        }
    },
    error: function (request, status, error) {
        console.log("request: " + request);
        console.log("status: " + status);
        console.log("ERROR: " + error);
    }

});
