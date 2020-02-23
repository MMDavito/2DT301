console.log("HURAY FOR ME!");
yesnoCheckProgram

function yesnoCheckProgram() {
    if (document.getElementById("noCheckProgram").checked) {
        $(".static_div").show();
        $(".dynamic_div").hide();
    } else {
        $(".static_div").hide();
        $(".dynamic_div").show();
    }
}
function yesnoCheck() {
    if (document.getElementById("noCheck").checked) {
        $(".div_start_time").hide();
    } else {
        $(".div_start_time").show();
    }
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
        var FormBajs = document.getElementById("FormBajs");
        var i = 0;
        data.forEach(element => {
            var textNode = document.createTextNode(JSON.stringify(element));
            var node = document.createElement("LI");
            node.appendChild(textNode);
            li.appendChild(node);

            //TODO: WIll try to make child of label id "ON" or "OFF".
            var formStatic = document.createElement("div");
            formStatic.classList.add("static_div");
            var formDynamic = document.createElement("div");
            formDynamic.classList.add("dynamic_div");
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
            formStatic.appendChild(labelON);

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
            formStatic.appendChild(labelOFF);
            var lineBreak = document.createElement("br");
            formStatic.appendChild(lineBreak);

            var textNode = document.createTextNode("Duration: ");
            var lineBreak = document.createElement("br");
            formDynamic.appendChild(textNode);
            formDynamic.appendChild(lineBreak);
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
            formDynamic.appendChild(label);
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
            formDynamic.appendChild(label);
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
            formDynamic.appendChild(label);
            var lineBreak = document.createElement("br");

            formDynamic.appendChild(lineBreak);


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
                divStartTime.classList.add("div_start_time");
                divStartTime.appendChild(label);
                formDynamic.appendChild(divStartTime);
            }
            else {
                formDynamic.appendChild(label);
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
                formDynamic.appendChild(lineBreak);
            }
            formDynamic.appendChild(label);
            formDiv.appendChild(formStatic);
            formDiv.appendChild(formDynamic)
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
        }else{
            $(".div_start_time").show();
        }
        
        if (document.getElementById("noCheckProgram").checked){
            $(".static_div").show();
            $(".dynamic_div").hide();
        }else{
            $(".static_div").hide();
            $(".dynamic_div").show();
        }
    },
    error: function (request, status, error) {
        console.log("request: " + request);
        console.log("status: " + status);
        console.log("ERROR: " + error);
    }

});
