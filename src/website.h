#ifndef _WEBSITE_H
#define _WEBSITE_H

static const char html[] = 
"<!DOCTYPE html>\
<html>\
    <head>\
        <meta charset='utf-8'>\
        <style>\
            body {\
                background-color: #3c3836;\
                color: #ebdbb2;\
                font-family: monospace;\
            }\
            #main {\
                display: flex;\
                flex-wrap:wrap;\
                row-gap: 10cqh;\
                column-gap: 5cqw;\
            }\
            #side {\
                display: flex;\
                flex-wrap:wrap;\
                justify-content: center;\
                margin-top: 15cqh;\
            }\
        </style>\
    </head>\
    <body>\
        <form id='main'></form>\
        <from id='side'>\
            <input id='setAll' type='color'>\
        </from>\
        <script>\
            var colors = {{f}};\
            var ins = {f};\
            var updated = false;\
            function updateColors(e) {\
                let id = e.target.id;\
                colors[id] = String(e.target.value);\
                updated = true;\
                return;\
            }\
            function setAll(e) {\
                let id = e.target.id;\
                for(let i = 0; i < ins; i++) {\
                    document.getElementById('in' + i).setAttribute('value', String(e.target.value));\
                    document.getElementById('in' + i).value = e.target.value;\
                    colors['in' + i] = String(e.target.value);\
                }\
                updated = true;\
                return;\
            }\
            function post() {\
                if(updated) {\
                    let colorStr = 'ftl';\
                    for(let i = 0; i < ins; i++) {\
                        colorStr += colors['in' + i][1];\
                        colorStr += colors['in' + i][2];\
                        colorStr += colors['in' + i][3];\
                        colorStr += colors['in' + i][4];\
                        colorStr += colors['in' + i][5];\
                        colorStr += colors['in' + i][6];\
                    }\
                    fetch('http://192.168.1.{f}', {\
                        method: 'POST',\
                        body: colorStr,\
                        headers: {\
                        'Content-type': 'text/plain; charset=UTF-8'\
                        }\
                    });\
                    updated = false;\
                }\
                return;\
            }\
            var body = document.getElementById('main');\
            for(let i = 0; i < ins; i++) {\
                let inp = document.createElement('input');\
                inp.setAttribute('type', 'color');\
                inp.setAttribute('value', colors['in' + i]);\
                inp.setAttribute('id', 'in' + i);\
                inp.addEventListener('input', updateColors);\
                body.appendChild(inp);\
            }\
            setInterval(post, 1000);\
            document.getElementById('setAll').addEventListener('input', setAll);\
        </script>\
    </body>\
</html>";
int htmlSize = sizeof(html) - 1;

#endif