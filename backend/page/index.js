(()=>{"use strict";var e={703:e=>{function t(e,n,r,a){this.message=e,this.expected=n,this.found=r,this.location=a,this.name="SyntaxError","function"==typeof Error.captureStackTrace&&Error.captureStackTrace(this,t)}!function(e,t){function n(){this.constructor=e}n.prototype=t.prototype,e.prototype=new n}(t,Error),t.buildMessage=function(e,t){var n={literal:function(e){return'"'+a(e.text)+'"'},class:function(e){var t,n="";for(t=0;t<e.parts.length;t++)n+=e.parts[t]instanceof Array?o(e.parts[t][0])+"-"+o(e.parts[t][1]):o(e.parts[t]);return"["+(e.inverted?"^":"")+n+"]"},any:function(e){return"any character"},end:function(e){return"end of input"},other:function(e){return e.description}};function r(e){return e.charCodeAt(0).toString(16).toUpperCase()}function a(e){return e.replace(/\\/g,"\\\\").replace(/"/g,'\\"').replace(/\0/g,"\\0").replace(/\t/g,"\\t").replace(/\n/g,"\\n").replace(/\r/g,"\\r").replace(/[\x00-\x0F]/g,(function(e){return"\\x0"+r(e)})).replace(/[\x10-\x1F\x7F-\x9F]/g,(function(e){return"\\x"+r(e)}))}function o(e){return e.replace(/\\/g,"\\\\").replace(/\]/g,"\\]").replace(/\^/g,"\\^").replace(/-/g,"\\-").replace(/\0/g,"\\0").replace(/\t/g,"\\t").replace(/\n/g,"\\n").replace(/\r/g,"\\r").replace(/[\x00-\x0F]/g,(function(e){return"\\x0"+r(e)})).replace(/[\x10-\x1F\x7F-\x9F]/g,(function(e){return"\\x"+r(e)}))}return"Expected "+function(e){var t,r,a,o=new Array(e.length);for(t=0;t<e.length;t++)o[t]=(a=e[t],n[a.type](a));if(o.sort(),o.length>0){for(t=1,r=1;t<o.length;t++)o[t-1]!==o[t]&&(o[r]=o[t],r++);o.length=r}switch(o.length){case 1:return o[0];case 2:return o[0]+" or "+o[1];default:return o.slice(0,-1).join(", ")+", or "+o[o.length-1]}}(e)+" but "+function(e){return e?'"'+a(e)+'"':"end of input"}(t)+" found."},e.exports={SyntaxError:t,parse:function(e,n){n=void 0!==n?n:{};var r,a,o,i,c={},s={rake:L},l=L,u=A(":",!1),d=A(",",!1),h=A("0",!1),f=A("1",!1),p=A("2",!1),v=A("3",!1),g=A("4",!1),m=A("5",!1),y=A("6",!1),x=A("7",!1),b=A("8",!1),w=A("9",!1),k=0,E=[{line:1,column:1}],C=0,B=[];if("startRule"in n){if(!(n.startRule in s))throw new Error("Can't start parsing from rule \""+n.startRule+'".');l=s[n.startRule]}function A(e,t){return{type:"literal",text:e,ignoreCase:t}}function M(t){var n,r=E[t];if(r)return r;for(n=t-1;!E[n];)n--;for(r={line:(r=E[n]).line,column:r.column};n<t;)10===e.charCodeAt(n)?(r.line++,r.column=1):r.column++,n++;return E[t]=r,r}function P(e,t){var n=M(e),r=M(t);return{start:{offset:e,line:n.line,column:n.column},end:{offset:t,line:r.line,column:r.column}}}function I(e){k<C||(k>C&&(C=k,B=[]),B.push(e))}function L(){var t,n,r,a;return t=k,(n=z())!==c?(58===e.charCodeAt(k)?(r=":",k++):(r=c,I(u)),r!==c&&(a=S())!==c?t=n={origin:n,spacing:a.reverse()}:(k=t,t=c)):(k=t,t=c),t===c&&(t=k,(n=S())!==c&&(n={origin:0,spacing:n.reverse()}),t=n),t}function S(){var t,n,r,a,o,i;return t=k,(n=N())!==c?(44===e.charCodeAt(k)?(r=",",k++):(r=c,I(d)),r!==c&&(a=S())!==c?(o=n,(i=a).push(o),t=n=i):(k=t,t=c)):(k=t,t=c),t===c&&(t=k,(n=N())!==c&&(n=function(e){return[parseInt(e)]}(n)),t=n),t}function z(){var e,t;for(k,e=[],t=_();t!==c;)e.push(t),t=_();return e!==c&&(e=parseInt(e.join(""))),e}function N(){return z()}function _(){var t;return 48===e.charCodeAt(k)?(t="0",k++):(t=c,I(h)),t===c&&(t=function(){var t;return 49===e.charCodeAt(k)?(t="1",k++):(t=c,I(f)),t===c&&(50===e.charCodeAt(k)?(t="2",k++):(t=c,I(p)),t===c&&(51===e.charCodeAt(k)?(t="3",k++):(t=c,I(v)),t===c&&(52===e.charCodeAt(k)?(t="4",k++):(t=c,I(g)),t===c&&(53===e.charCodeAt(k)?(t="5",k++):(t=c,I(m)),t===c&&(54===e.charCodeAt(k)?(t="6",k++):(t=c,I(y)),t===c&&(55===e.charCodeAt(k)?(t="7",k++):(t=c,I(x)),t===c&&(56===e.charCodeAt(k)?(t="8",k++):(t=c,I(b)),t===c&&(57===e.charCodeAt(k)?(t="9",k++):(t=c,I(w)))))))))),t}()),t}if((r=l())!==c&&k===e.length)return r;throw r!==c&&k<e.length&&I({type:"end"}),a=B,o=C<e.length?e.charAt(C):null,i=C<e.length?P(C,C+1):P(C,C),new t(t.buildMessage(a,o),a,o,i)}}}},t={};function n(r){var a=t[r];if(void 0!==a)return a.exports;var o=t[r]={exports:{}};return e[r](o,o.exports,n),o.exports}n.n=e=>{var t=e&&e.__esModule?()=>e.default:()=>e;return n.d(t,{a:t}),t},n.d=(e,t)=>{for(var r in t)n.o(t,r)&&!n.o(e,r)&&Object.defineProperty(e,r,{enumerable:!0,get:t[r]})},n.o=(e,t)=>Object.prototype.hasOwnProperty.call(e,t),(()=>{var e=n(703);const t=parseInt,r={};window.Storage=class{constructor(){const e=window.localStorage;e.getItem("store-active")?(this.fetch=function(t){return e.getItem(t)},this.store=function(t,n){r[t]=n,e.setItem(t,n)},Object.keys(r).forEach((function(e){this.store(e,r[e])}),this)):(this.fetch=function(){return null},this.store=function(e,t){r[e]=t})}};class a{constructor(t){this.config=e.parse(t),this.patternWidth=this.config.spacing.reduce((function(e,t){return e+t+1}),0),this.pattern=new Array(this.patternWidth).fill(0);const n=this.pattern;var r=0;this.config.spacing.forEach((function(e){n[r]=1,r+=e})),this.createPattern=function(){},this.getNails=function(e){return[...new Array(1e3).keys()].map((function(t){return t-(e+10*this.config.origin)}),this).map((function(e){return 0==Math.abs(e%10)?(e=Math.floor(e/10))>=0?e%this.patternWidth:this.patternWidth-1+(e+1)%this.patternWidth:-1}),this).map((function(e){return-1==e?0:this.pattern[e]}),this)}}}var o={};const c=["draw","rake"];var s="draw",l={},u={fn_bind:!1,dom_setup:!1,init:!1},d=2263842,h={};function f(e){return"#"==e.charAt(0)?t("0x"+e.substr(1)):e.match("rgb\\((\\d*), (\\d*), (\\d*)").slice(1,4).reduce((function(e,n){return 256*e+t(n)}),0)}function p(){if(u.init||!u.fn_bind||!u.dom_setup)return;u.setBGColor(f(M.image.style.backgroundColor)),h[h.active].id=u.addPallete(3);const{A:e,B:t,C:n}=h[h.active];u.setPaletteColors(f(e),f(t),f(n)),u.setActivePalette(h[h.active].id),u.redraw(),u.init=!0}function v(){const{A:e,B:t,C:n}=h[h.active];h.nodes.A.style.background=e,h.nodes.B.style.background=t,h.nodes.C.style.background=n}window.Module={onRuntimeInitialized:function(){u.addPallete=Module.cwrap("addPalette","number",["number"]),u.setActivePalette=Module.cwrap("setActivePalette","number",["number"]),u.setPaletteColors=Module.cwrap("setPaletteColors","number",["number","number","number","number"]),u.setBGColor=Module.cwrap("setBGColor","void",["number"]),u.addDrop=Module.cwrap("addDrop","number",["number","number","number","number"]),u.resizeDrop=Module.cwrap("resizeDrop","number",["number","number"]),u.setColorAt=Module.cwrap("setColorAt","number",["number","number"]),u.redraw=Module.cwrap("redraw","void",[]),u.startRaking=Module.cwrap("startRaking","void",[]),u.rakeLinear=Module.cwrap("rakeLinear","boolean",["number","number","number","array"]),u.finishDrop=Module.cwrap("finishDrop","number",["number"]),u.clearCanvas=Module.cwrap("clearCanvas","void",[]),u.fn_bind=!0,p()}};var g={rake_dropper:{},lost_focus:function(){var e,t;e=g.menu.getBoundingClientRect(),(t=M.overlay.getBoundingClientRect()).left>e.right||t.right<e.left||t.top>e.bottom||t.bottom<e.top||(g.btn.checked=!1)}};function m(e,t){l[e]=[],Array.prototype.forEach.call(document.getElementsByClassName("state "+e),(function(t){var n=t,r=!1;do{for(n=n.nextSibling;n&&"LI"!=n.tagName;)n=n.nextSibling;r=!1,n&&c.forEach((function(e){n.classList.contains(e)&&(r=!0)}))}while(r);l[e].push({node:t,par:t.parentNode,next:n})})),l[t]&&l[t].forEach((function(e){e.par.insertBefore(e.node,e.next)})),l[e].forEach((function(e){e.node.remove()})),"rake"===t&&u.startRaking()}var y=null;function x(e){switch(e.id){case"download":!function(){const e=Module.ccall("getImage","number",[],[]),t=Module.HEAPU8[e]+1555200+40,n=Module.HEAPU8.slice(e+1,e+t),r=new Blob([n],{type:"image/x-portable-pixmap"});y||(y=document.createElement("a"),document.body.appendChild(y),y.style.display="none");const a=window.URL.createObjectURL(r);y.href=a,y.download="marebllous-image.ppm",y.click()}();break;case"clear":u.clearCanvas();break;case"color-1":d="A";break;case"color-2":d="B";break;case"color-3":d="C";break;case"color-dropper":M.tool[s]=w;break;case"color-rake":M.tool[s]=C;break;case"color-rake-displaced":C.config={w:150,h:150,of:75};break;case"color-rake-square":C.config={w:150,h:150,of:0};break;case"color-rake-custom":C.config=g.rake_dropper,g.btn.checked=!0,g.options.rake_dropper.checked=!0;break;case"color-sprinkler":M.tool[s]=k;break;case"rake-movement-linear":B.config.line=B.straight;break;case"rake-movement-wave":B.config.line=B.curve,B.config.magnitude=50,B.config.periode=200;break;case"switch-state":const t=e.parentNode.querySelector("label"),n=t.querySelector("img"),r=s;switch(s){case"draw":s="rake",n.src="icons/fast-backward-solid.svg",t.title="Go to state before first rake strike.";break;case"rake":s="draw",t.title="Start rakeing.",n.src="icons/arrow-right-solid.svg"}m(r,s)}}function b(e,t){const n=storage.fetch(t);n&&(e.value=n)}document.addEventListener("click",(function(e){var t=e.target;if("INPUT"==t.tagName&&t.attributes.type&&"radio"==t.attributes.type.nodeValue&&!t.parentNode.parentNode.classList.contains("sidebar")){x(t);const e=function(e){var t,n=e;if(n.parentNode.parentNode.classList.contains("menu"))n.parentNode.childNodes.forEach((function(e){"MENU"==e.tagName&&(t=e)}));else for(;!n.classList.contains("menu");)"MENU"==n.tagName&&(t=n),n=n.parentNode;return t}(t);o[t.attributes.name.nodeValue]==t?e&&e.querySelectorAll("li").forEach((function(e){e.classList.toggle("hide")})):(o[t.attributes.name.nodeValue]=t,e&&(e.querySelectorAll("li").forEach((function(e){e.classList.remove("hide")})),e.attributes.option&&(g.options[e.attributes.option.value].checked=!0)))}else"INPUT"==t.tagName&&t.attributes.type&&"button"==t.attributes.type.nodeValue&&x(t)})),document.addEventListener("touchstart",(function(){alert("No support for touch inputs at the moment!")})),window.storage=null,document.addEventListener("DOMContentLoaded",(function(){storage=new Storage,document.querySelectorAll('menu input[type="radio"]').forEach((function(e){e.checked?(o[e.attributes.name.nodeValue]=e,x(e)):void 0===o[e.attributes.name.nodeValue]&&(o[e.attributes.name.nodeValue]=null)})),M.image=document.getElementById("image"),M.overlay=document.getElementById("overlay"),M.overlay.addEventListener("mousedown",(function(e){!function(){const e=o.menu;if(e){const n=o[e.id];if(n){var t=n;do{t=o[t.id]}while(t);void 0===t&&(n.classList.contains("hide")||e.parentNode.querySelectorAll("li").forEach((function(e){e.classList.add("hide")})))}}}(),g.lost_focus(),M.click(e)})),M.overlay.addEventListener("mousemove",M.over),M.overlay.addEventListener("mouseup",M.up),M.overlay.addEventListener("mousedown",M.down),M.overlay.addEventListener("mouseleave",M.leave),M.overlay.addEventListener("contextmenu",(function(e){e.preventDefault()})),c.forEach((function(e){m(e,null)})),m(null,s),w.img=document.getElementById("img-color-dropper"),k.img=document.getElementById("img-color-sparkle"),document.querySelectorAll("menu.sidebar > li > div").forEach((function(e){e.style.maxHeight=e.scrollHeight+3})),g.options={rake:document.getElementById("sidebar-rake"),pallet:document.getElementById("sidebar-pallet")},g.btn=document.getElementById("sidebar-btn"),g.menu=document.querySelector("menu.sidebar");{const e="sidebar-pallet-active-pallet",t=document.getElementById(e);b(t,e),h.active=t.value,t.addEventListener("change",(n=>{h.active=t.value,storage.store(e,t.value),void 0===h[h.active]?(h[h.active]={id:u.addPallete(3),A:null,B:null,C:null},u.setActivePalette(h[h.active].id)):u.setActivePalette(h[h.active].id);var{A:r,B:a,C:o,background:i}=h[h.active];null==r?b(h.inputs.A,h.active+"sidebar-pallet-color-1"):h.inputs.A.value=r,null==a?b(h.inputs.B,h.active+"sidebar-pallet-color-2"):h.inputs.B.value=a,null==o?b(h.inputs.C,h.active+"sidebar-pallet-color-3"):h.inputs.C.value=o,null==i?b(h.inputs.background,h.active+"sidebar-pallet-background"):h.inputs.background.value=i,r=h.inputs.A.value,a=h.inputs.B.value,o=h.inputs.C.value,i=h.inputs.background.value,h[h.active]={A:r,B:a,C:o,background:i},u.setPaletteColors(f(r),f(a),f(o)),u.setBGColor(f(i)),u.redraw(),v()})),h.inputs={},null==h[h.active]&&(h[h.active]={})}{const e="sidebar-pallet-background",t=document.getElementById(e),n=function(){return h.active+e};b(t,n()),h[h.active].background=t.value,t.addEventListener("change",(e=>{h[h.active].background=t.value,storage.store(n(),t.value),u.setBGColor(f(t.value)),u.redraw()})),h.inputs.background=t,h[h.active].background=t.value}{const e="sidebar-pallet-color-1",t=document.getElementById(e),n=function(){return h.active+e};b(t,n()),h[h.active].A=t.value,t.addEventListener("change",(e=>{h[h.active].A=t.value,storage.store(n(),t.value),u.setColorAt(0,f(t.value)),u.redraw(),v()})),h.inputs.A=t,h[h.active].A=t.value}{const e="sidebar-pallet-color-2",t=document.getElementById(e),n=function(){return h.active+e};b(t,n()),h[h.active].B=t.value,t.addEventListener("change",(e=>{h[h.active].B=t.value,storage.store(n(),t.value),u.setColorAt(1,f(t.value)),u.redraw(),v()})),h.inputs.B=t,h[h.active].B=t.value}{const e="sidebar-pallet-color-3",t=document.getElementById(e),n=function(){return h.active+e};b(t,n()),h[h.active].C=t.value,t.addEventListener("change",(e=>{h[h.active].C=t.value,storage.store(n(),t.value),u.setColorAt(2,f(t.value)),u.redraw(),v()})),h.inputs.C=t,h[h.active].C=t.value}h.nodes={A:document.getElementById("color-1").labels[0],B:document.getElementById("color-2").labels[0],C:document.getElementById("color-3").labels[0]},v();{const e="sidebar-rake-placement",t=document.getElementById(e);b(t,e),B.config.placement=new a(t.value),t.addEventListener("change",(n=>{try{B.config.placement=new a(t.value),storage.store(e,t.value)}catch(e){}})),t.addEventListener("keydown",(e=>{13==e.which&&t.blur()}))}u.dom_setup=!0,p()}));var w={circle:null,active:!1,speed:2e-5,fnSwitch:100,slope:null,slowF:3,time:null,lastdrop:null,draw:function(e,t,n){const r=M.translate({x:32,y:32});e.drawImage(w.img,t,n-r.y,r.x,r.y)},drop:function(e){if(null===w.slope&&(w.slope=Math.sqrt(w.fnSwitch*w.speed)/w.fnSwitch),w.active){w.time||(w.time=e);var t=Math.sqrt((e-w.time-w.fnSwitch*(w.slowF-1))*w.speed);e-w.time<w.fnSwitch*w.slowF&&(t=(e-w.time)*w.slope/w.slowF),u.resizeDrop(w.circle,t),window.requestAnimationFrame(w.drop)}else u.finishDrop(w.drop)},down:function(e,t,n,r){var a=0;switch(d){case"A":a=0;break;case"B":a=1;break;case"C":a=2}const o={x:2*e/n-1,y:1-2*t/r,r:0};w.circle=u.addDrop(o.x,o.y,o.r,a),w.active=!0,window.requestAnimationFrame(w.drop)},up:function(){w.active=!1,w.time=null}},k={draw:function(e,t,n){const r=M.translate({x:32,y:32});e.drawImage(k.img,t-r.x/2,n-r.y/2,r.x,r.y)}};function E(e,t,n,r){var a,o,i,c=n.x,s=n.y;e.beginPath(),e.moveTo(t.x,t.y),e.lineTo(n.x,n.y),e.stroke(),e.beginPath(),a=Math.atan2(n.y-t.y,n.x-t.x),o=r*Math.cos(a)+c,i=r*Math.sin(a)+s,e.moveTo(o,i),a+=1/3*(2*Math.PI),o=r*Math.cos(a)+c,i=r*Math.sin(a)+s,e.lineTo(o,i),a+=1/3*(2*Math.PI),o=r*Math.cos(a)+c,i=r*Math.sin(a)+s,e.lineTo(o,i),e.closePath(),e.fill()}var C={canvas:void 0,ctx:void 0,config:{w:30,h:30,of:15},init:function(){C.canvas||C.ctx||(C.canvas=document.createElement("canvas"),C.canvas.width=32,C.canvas.height=32,C.ctx=C.canvas.getContext("2d"))},setPattern:function(e){void 0===this.config_last&&(this.config_last={});var t=!1;if(Object.keys(C.config).forEach((function(e){C.config_last[e]!==C.config[e]&&(t=!0,C.config_last[e]=C.config[e])})),t){C.init();const t=e.canvas.width*this.config.w/1e3,n=e.canvas.height*this.config.h/1e3,r=e.canvas.height*this.config.of/1e3%n,a=5;this.size={x:a,y:a},this.pattern_size={x:2*t,y:n};const o={x:t,y:r};C.canvas.width=this.pattern_size.x,C.canvas.height=this.pattern_size.y,C.ctx.beginPath(),C.ctx.ellipse(this.size.x,this.size.y,this.size.x,this.size.y,0,2*Math.PI,!1),C.ctx.moveTo(this.size.x+o.x+this.size.x,this.size.y+o.y),C.ctx.ellipse(this.size.x+o.x,this.size.y+o.y,this.size.x,this.size.y,0,2*Math.PI,!1),C.ctx.stroke(),this.pattern=e.createPattern(C.canvas,"repeat")}return e.fillStyle=this.pattern,{size:this.size,pattern_size:this.pattern_size}},draw:function(e,t,n,r,a){const{size:o,pattern_size:i}=C.setPattern(e);t%=i.x,n%=i.y,e.translate(t-o.x,n-o.y),e.fillRect(-t,-n,r+t,a+n),e.setTransform(1,0,0,1,0,0)}},B={borderPoint:function(e,t,n,r,a){const o=e+t*a;return o<0?{y:0,x:(0-e)/t}:o>=r?{y:r,x:(r-e)/t}:{y:o,x:a}},lrBound:function(e,t,n,r){const a=t.x-e.x,o=(t.y-e.y)/a;if(Math.abs(o)===1/0)return{leftBound:{x:e.x,y:0},rightBound:{x:e.x,y:r}};const i=e.y-o*e.x;return{leftBound:B.borderPoint(i,o,n,r,0),rightBound:B.borderPoint(i,o,n,r,n)}},straight:function(e,t,n,r,a){const{leftBound:o,rightBound:i}=B.lrBound(t,n,r,a);e.lineWidth=.5,e.beginPath(),e.moveTo(o.x,o.y),e.lineTo(i.x,i.y),e.stroke(),e.lineWidth=2,e.strokeStyle="black",e.fillStyle="black",E(e,t,n,10)},curve:function(e,t,n,r,a){const{leftBound:o,rightBound:c}=B.lrBound(t,n,r,a);var s,l;n.x-t.x>0||n.x===t.x&&t.y<n.y?(s={x:c.x-o.x,y:c.y-o.y},l=o):(s={x:o.x-c.x,y:o.y-c.y},l=c);const u=Math.sqrt(s.x*s.x+s.y*s.y),d=Math.sqrt((t.x-l.x)**2+(t.y-l.y)**2),h=-s.y/u,f=s.x/u;e.lineWidth=.5,e.beginPath();{const t=-B.config.periode/4-20,n=Math.sin((t-d)/B.config.periode*Math.PI)*B.config.magnitude,r=l.x+t/u*s.x,a=l.y+t/u*s.y;e.moveTo(r+h*n,a+f*n)}for(i=-B.config.periode/4;i<=u+B.config.periode/4+20;i+=20){const t=l.x+i/u*s.x,n=l.y+i/u*s.y,r=Math.sin((i-d)/B.config.periode*Math.PI)*B.config.magnitude;e.lineTo(t+h*r,n+f*r)}e.stroke(),e.lineWidth=2,E(e,t,n,10)},config:{of:5,line:function(e,t,n,r,a){B.straight(e,t,n,r,a)}},init:function(){B.canvas||B.ctx||(B.canvas=document.createElement("canvas"),B.canvas.width=32,B.canvas.height=32,B.ctx=B.canvas.getContext("2d"))},setPattern:function(e){if(this.placement!==B.config.placement){console.log(B.config.placement),this.placement=B.config.placement,B.init();const n=5;this.size={x:n,y:n};const r=e.canvas.width/100,a=this.placement.patternWidth*r;B.canvas.width=a,B.canvas.height=2*this.size.y,B.ctx.setTransform(1,0,0,1,0,0),B.ctx.beginPath();{var t=n;const e=n;B.config.placement.config.spacing.forEach((function(a){B.ctx.moveTo(t+n,e),B.ctx.ellipse(t,e,n,n,0,2*Math.PI,!1),t+=(1+a)*r}))}B.ctx.stroke(),this.pattern=e.createPattern(B.canvas,"repeat")}return e.strokeStyle=this.pattern,this.size.x},onemove:function(e,t,n,r,a){B.setPattern(e);const o={y:-n.x+t.x,x:n.y-t.y},i=B.setPattern(e),c=Math.atan2(o.y,o.x),{leftBound:s,rightBound:l}=B.lrBound(t,{x:t.x+o.x,y:t.y+o.y},r,a),u=Math.sqrt((s.x-l.x)**2+(s.y-l.y)**2),d=o.x>0||0===o.x&&o.y>0?s:l,h=e.canvas.width/100,f=Math.sqrt((t.x-d.x)**2+(t.y-d.y)**2),p=(-(NaN==f?0:f)+this.placement.config.origin*h)%B.canvas.width;e.lineWidth=2*i,e.beginPath(),e.translate(d.x,d.y),e.rotate(c),e.translate(-i-p,-i),e.moveTo(0+p,i),e.lineTo(u+p,i),e.stroke(),e.setTransform(1,0,0,1,0,0),e.strokeStyle="black",B.config.line(e,t,n,r,a)},up:function(e,t,n,r){if(!e||!t)return;const a={x:t.x-e.x,y:t.y-e.y},o=Math.sqrt(a.x*a.x+a.y*a.y);var i;i=Math.abs(a.x)>Math.abs(a.y)?Math.floor(e.y/r*1e3):Math.floor(e.x/n*1e3),u.rakeLinear(a.x/n,a.y/r,o,B.config.placement.getNails(i))}};function A(e,t){const n={x:t.x-e.x,y:t.y-e.y};return Math.abs(n.x)>Math.abs(n.y)?n.y=0:n.x=0,[e,{x:e.x+n.x,y:e.y+n.y}]}var M={start:null,overlay:null,tool:{draw:w,rake:B},leave:function(){M.clear(),M.tool[s].up&&M.tool[s].up()},clear:function(){M.ctx&&M.ctx.clearRect(0,0,M.overlay.width,M.overlay.height)},scale:function(){const e=M.overlay.getBoundingClientRect();return{x:M.overlay.width/e.width,y:M.overlay.height/e.height}},translate:function(e){const t=M.scale();return{x:e.x*t.x,y:e.y*t.y}},click:function(e){},down:function(e){M.start=M.translate({x:e.offsetX,y:e.offsetY}),M.tool[s].down&&M.tool[s].down(M.start.x,M.start.y,M.overlay.width,M.overlay.height)},over:function(e){if(M.pos=M.translate({x:e.offsetX,y:e.offsetY}),M.ctx||(M.ctx=M.overlay.getContext("2d")),M.ctx){M.clear();const e=M.pos;if(null!==M.start&&M.tool[s].onemove){const[t,n]=A(M.start,e);M.tool[s].onemove(M.ctx,t,n,M.overlay.width,M.overlay.height)}else M.tool[s].draw&&M.tool[s].draw(M.ctx,e.x,e.y,M.overlay.width,M.overlay.height)}},up:function(e){if(M.tool[s].up)if(M.start&&M.pos){const[e,t]=A(M.start,M.pos);M.tool[s].up(e,t,M.overlay.width,M.overlay.height)}else M.tool[s].up();M.start=null}}})()})();