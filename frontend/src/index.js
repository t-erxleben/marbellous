import * as parser from './rake_syntax.pegjs'


const int = parseInt;

window.Storage =  class Storage {
	constructor() {
		const store = window.localStorage;
		if(store.getItem('store-active'))	{
			this.fetch = function(id) {
				return store.getItem(id)
			}
			this.store = function(id, value) {
				store.setItem(id, value)
			}
		} else {
			this.fetch = function() { return null; }
			this.store = function() {}
		}
	}
}



class RakeConfig {
	constructor(term) {
		this.config = parser.parse(term);
		this.patternWidth = this.config.spacing.reduce(function (a,b){ return a + b + 1; }, 0);
		this.pattern = new Array(this.patternWidth).fill(0)
		const pat = this.pattern
		var sum = 0
		this.config.spacing.forEach(
			function(x) {
				pat[sum] = 1
				sum += x
			}
		)
		this.createPattern = function() {}
		this.getNails = function(handle) {
			const width = 1000
			return new Array(width).map(function(x,i) {
				const d = i - handle + origin
				if(d > 0 && d % 10 == 0)  {
					return this.pattern[d % this.patternWidth]
				} else if ( d < 0 && d % 10 == 0) {
					return this.pattern[this.patternWidth - 1 - (d % this.patternWidth)]
				} else {
					return 1
				}
			})
		}
	}
}

var active = {};
const states = ['draw','rake'];
var state = 'draw';
var nodes = {};
var backend = {fn_bind: false, dom_setup: false, init: false};
var color = 0x228B22;
var pallets = {}
window.Module = {
	onRuntimeInitialized: function() {
		backend.addPallete = Module.cwrap('addPalette',
			'number', ['number']);
		backend.setActivePalette = Module.cwrap('setActivePalette',
			'number', ['number']);
		backend.setPaletteColors = Module.cwrap('setPaletteColors',
			'number', ['number', 'number', 'number', 'number']);
		backend.setBGColor = Module.cwrap('setBGColor',
			'void', ['number']);
		backend.addDrop = Module.cwrap('addDrop',
			'number', ['number', 'number', 'number', 'number']);
		backend.resizeDrop = Module.cwrap('resizeDrop',
			'number', ['number', 'number']);
		backend.setColorAt = Module.cwrap('setColorAt',
			'number', ['number', 'number']);
		backend.redraw = Module.cwrap('redraw',
			'void', []);
		backend.startRaking = Module.cwrap('startRaking', 'void', [])
		backend.rakeLinear = Module.cwrap('rakeLinear',
			'boolean', ['number', 'number', 'number', 'array'])
		backend.finishDrop = Module.cwrap('finishDrop', 'number', ['number'])
		backend.clearCanvas = Module.cwrap('clearCanvas', 'void', [])
		backend.fn_bind = true;
		init();
	}
};
function color2int(color) {
	if(color.charAt(0) == '#')	
		return int("0x" + color.substr(1));
	return color.match('rgb\\((\\d*), (\\d*), (\\d*)')
		.slice(1,4).reduce(function(acc,el){return acc*256+int(el)}, 0)
}
function init() {
	if(backend.init || !(backend.fn_bind && backend.dom_setup)) return;
	backend.setBGColor(color2int(tool.image.style.backgroundColor));
	pallets[pallets.active].id = backend.addPallete(3);
	const {A, B, C} = pallets[pallets.active]
	backend.setPaletteColors(color2int(A), color2int(B), color2int(C))
	backend.setActivePalette(pallets[pallets.active].id);
	backend.redraw();
	backend.init = true;
}

function intersectRect(r1, r2) {
  return !(r2.left > r1.right || 
           r2.right < r1.left || 
           r2.top > r1.bottom ||
           r2.bottom < r1.top);
}
function updatePallet() {
	const {A, B, C} = pallets[pallets.active];
	pallets.nodes.A.style.background = A;
	pallets.nodes.B.style.background = B;
	pallets.nodes.C.style.background = C;
}
var sidebar = {
	rake_dropper: {},
	lost_focus: function(){
		const r1 = sidebar.menu.getBoundingClientRect();
		const r2 = tool.overlay.getBoundingClientRect();
		if(intersectRect(r1,r2)) {
			sidebar.btn.checked = false;
		}
	}
};

function getTopSubmenu(label) {
	var el = label;
	var menu;
	if (el.parentNode.parentNode.classList.contains('menu')) {
		el.parentNode.childNodes.forEach(function(node){
			if (node.tagName == 'MENU') {
				menu = node;
			}
		});
	} else {
		while(!el.classList.contains('menu')) {
			if(el.tagName=='MENU') {
				menu = el;
			}
			el = el.parentNode;
		}
	}
	return menu;
}

function switchState(_old, _new) {
	nodes[_old] = []
	Array.prototype.forEach.call(document.getElementsByClassName('state ' + _old), function (e){
		var next = e;
		var hit = false;
		do{
			next = next.nextSibling;
			while(next && next.tagName != 'LI') { next = next.nextSibling; }
			hit = false;
			if(next) {
				states.forEach(function(s){
					if (next.classList.contains(s)) { hit = true; }
				});
			}
		}while(hit);
		nodes[_old].push({node: e, par: e.parentNode, next});
	});
	if(nodes[_new]) {
		nodes[_new].forEach(function(e){
			e.par.insertBefore(e.node, e.next);
		});
	}
	nodes[_old].forEach(function(e){
		e.node.remove();
	});
	if(_new === 'rake') {
		backend.startRaking()
	}

}
var download_side = null;
function downloadCanvas() {
	const ptr = Module.ccall("getImage", "number", [], []);	
	const len = Module.HEAPU8[ptr] + 720*720*3 + 40;
	const data = Module.HEAPU8.slice(ptr+1, ptr+len);
	const blob = new Blob([data], {type: 'image/x-portable-pixmap'});
	if(!download_side) {
		download_side = document.createElement('a');
		document.body.appendChild(download_side);
		download_side.style.display = 'none';
	}
	const url = window.URL.createObjectURL(blob);
	download_side.href = url;
	download_side.download = "marebllous-image.ppm";
	download_side.click();
}


function handleClick(el) {
		switch(el.id) {
			case 'download':
				downloadCanvas();
				break;
			case 'clear':
				backend.clearCanvas()
				break;
			case 'color-1':
				color = 'A';
				break;
			case 'color-2':
				color = 'B';
				break;
			case 'color-3':
				color = 'C';
				break;
			case 'color-dropper':
				tool.tool[state] = dropper;
				break;
			case 'color-rake':
				tool.tool[state] = rake_dropper;
				break;
			case 'color-rake-displaced':
				rake_dropper.config = {
					w: 150,
					h: 150,
					of: 75,
				};
				break;
			case 'color-rake-square':
				rake_dropper.config = {
					w: 150,
					h: 150,
					of: 0,
				};
				break;
			case 'color-rake-custom':
				rake_dropper.config = sidebar.rake_dropper;
				sidebar.btn.checked = true;
				sidebar.options.rake_dropper.checked = true
				break;
			case 'color-sprinkler':
				tool.tool[state] = sparkle_dropper;
				break;
			case 'rake-movement-linear':
				rake.config.line = rake.straight;
				break;
			case 'rake-movement-wave':
				rake.config.line = rake.curve;
				rake.config.magnitude = 50;
				rake.config.periode = 200;
				break;
			case 'switch-state':
				const label = el.parentNode.querySelector('label');
				const img = label.querySelector('img');
				const oldState = state;
				switch(state) {
					case 'draw':
						state = 'rake';
						img.src = 'icons/fast-backward-solid.svg';
						label.title = 'Go to state before first rake strike.';
						break;
					case 'rake':
						state = 'draw';
						label.title = 'Start rakeing.'; // TODO: fix spelling
						img.src = 'icons/arrow-right-solid.svg'; break;
				}
				switchState(oldState, state);
		}
}


document.addEventListener("click", function(evnt){
	var el = evnt.target;
	// hide submenu if element is double clicked, show it again when clicked again or first selected
	if (el.tagName == 'INPUT'
		&& el.attributes.type
		&& el.attributes.type.nodeValue == 'radio'
		&& !el.parentNode.parentNode.classList.contains('sidebar'))
	{
		handleClick(el);
		const menu = getTopSubmenu(el);
		if (active[el.attributes.name.nodeValue] == el) {
			if (menu) {
				menu.querySelectorAll('li').forEach(function(li){
					li.classList.toggle('hide')});
			}
		} else {
			active[el.attributes.name.nodeValue] = el;
			if (menu) {
				menu.querySelectorAll('li').forEach(function(li){li.classList.remove('hide')});
				if(menu.attributes.option) {
					sidebar.options[menu.attributes.option.value].checked = true;
				}
			}
		}
	}
	else if (el.tagName == 'INPUT'
		&& el.attributes.type
		&& el.attributes.type.nodeValue == 'button')
	{
		handleClick(el);
	}
});
document.addEventListener("touchstart", function() {
	alert("No support for touch inputs at the moment!");	
});
function hideMenu() {
	const submenu = active['menu'];
	if(submenu) {
		const subsubmenu = active[submenu.id];
		if (subsubmenu) {
			var subsub = subsubmenu;
			do{
				subsub = active[subsub.id];
			} while(subsub);
			if(subsub === undefined) {
				if(!subsubmenu.classList.contains('hide')) {
					submenu.parentNode.querySelectorAll('li').forEach(function(li){li.classList.add('hide')});
				}
			}
		}
	}
}
function fetchAndSet(element, id) {
	const val = storage.fetch(id);
	if (val) { element.value = val; }
}
window.storage = null
document.addEventListener("DOMContentLoaded", function(){
	storage = new Storage()
	document.querySelectorAll('menu input[type="radio"]').forEach(function(rad){
		if (rad.checked) {
			active[rad.attributes.name.nodeValue] = rad;
			handleClick(rad);
		} else if(active[rad.attributes.name.nodeValue] === undefined) {
			active[rad.attributes.name.nodeValue] = null;
		}
	});
	{
		tool.image = document.getElementById('image');
	}
	tool.overlay = document.getElementById('overlay');
	tool.overlay.addEventListener("mousedown", function(evnt){
		hideMenu();
		sidebar.lost_focus();
		tool.click(evnt);
	});
	tool.overlay.addEventListener("mousemove", tool.over);
	tool.overlay.addEventListener("mouseup", tool.up);
	tool.overlay.addEventListener("mousedown", tool.down);
	tool.overlay.addEventListener("mouseleave", tool.leave);
	tool.overlay.addEventListener("contextmenu", function(e){e.preventDefault();});


	states.forEach(function(s){
		switchState(s, null);
	});
	switchState(null, state);
	dropper.img = document.getElementById('img-color-dropper');
	sparkle_dropper.img = document.getElementById('img-color-sparkle');

	document.querySelectorAll('menu.sidebar > li > div').forEach(function(div){
		div.style.maxHeight = div.scrollHeight + 3;
	});
	sidebar.options = {
		rake: document.getElementById('sidebar-rake'),
		pallet: document.getElementById('sidebar-pallet'),
	};
	sidebar.btn = document.getElementById('sidebar-btn');
	sidebar.menu = document.querySelector('menu.sidebar');

	{
		const id = 'sidebar-pallet-active-pallet'
		const el = document.getElementById(id);
		fetchAndSet(el, id);
		pallets.active = el.value;
		el.addEventListener("change", (ev)=>{pallets.active = el.value
			storage.store(id, el.value);
			if(pallets[pallets.active] === undefined) {
				pallets[pallets.active] = {id: backend.addPallete(3), A: null, B: null, C: null};
				backend.setActivePalette(pallets[pallets.active].id);
			} else {
				backend.setActivePalette(pallets[pallets.active].id);
			}
			var {A, B, C, background} = pallets[pallets.active];
			if(A == null) { fetchAndSet(pallets.inputs.A, pallets.active + 'sidebar-pallet-color-1') }
			else { pallets.inputs.A.value = A }
			if(B == null) { fetchAndSet(pallets.inputs.B, pallets.active + 'sidebar-pallet-color-2') }
			else { pallets.inputs.B.value = B }
			if(C == null) { fetchAndSet(pallets.inputs.C, pallets.active + 'sidebar-pallet-color-3') }
			else { pallets.inputs.C.value = C }
			if(background == null) { fetchAndSet(pallets.inputs.background, pallets.active + 'sidebar-pallet-background') }
			else { pallets.inputs.background.value = background }
			A = pallets.inputs.A.value;
			B = pallets.inputs.B.value;
			C = pallets.inputs.C.value;
			background = pallets.inputs.background.value;
			pallets[pallets.active] = {A, B, C, background}
			backend.setPaletteColors(color2int(A), color2int(B), color2int(C));
			backend.setBGColor(color2int(background))
			backend.redraw();
			updatePallet();
		});
		pallets.inputs = {};
		if(pallets[pallets.active] == undefined) {
			pallets[pallets.active] = {}
		}
	}
	{
		const iid = 'sidebar-pallet-background'
		const el = document.getElementById(iid)
		const id = function() { return pallets.active + iid }
		fetchAndSet(el, id())
		pallets[pallets.active]['background'] = el.value
		el.addEventListener("change", (ev)=>{pallets[pallets.active]['background'] = el.value;
			storage.store(id(), el.value);
			backend.setBGColor(color2int(el.value))
			backend.redraw()
		})
		pallets.inputs.background = el
		pallets[pallets.active].background = el.value
	}
	{
		const iid = 'sidebar-pallet-color-1';
		const el = document.getElementById(iid);
		const id = function() { return pallets.active + iid }
		fetchAndSet(el, id());
		pallets[pallets.active]['A'] = el.value;
		el.addEventListener("change", (ev)=>{pallets[pallets.active]['A'] = el.value;
			storage.store(id(), el.value);
			backend.setColorAt(0, color2int(el.value));
			backend.redraw();
			updatePallet();});
		pallets.inputs.A = el;
		pallets[pallets.active].A = el.value
	}
	{
		const iid = 'sidebar-pallet-color-2'
		const el = document.getElementById(iid);
		const id = function() { return pallets.active + iid }
		fetchAndSet(el, id())
		pallets[pallets.active]['B'] = el.value;
		el.addEventListener("change", (ev)=>{pallets[pallets.active]['B'] = el.value;
			storage.store(id(), el.value);
			backend.setColorAt(1, color2int(el.value));
			backend.redraw();
			updatePallet();});
		pallets.inputs.B = el;
		pallets[pallets.active].B = el.value
	}
	{
		const iid = 'sidebar-pallet-color-3'
		const el = document.getElementById(iid);
		const id = function() { return pallets.active + iid }
		fetchAndSet(el, id())
		pallets[pallets.active]['C'] = el.value;
		el.addEventListener("change", (ev)=>{pallets[pallets.active]['C'] = el.value;
			storage.store(id(), el.value)
			backend.setColorAt(2, color2int(el.value));
			backend.redraw();
			updatePallet();});
		pallets.inputs.C = el;
		pallets[pallets.active].C = el.value
	}
	pallets.nodes = {
		A: document.getElementById('color-1').labels[0],
		B: document.getElementById('color-2').labels[0],
		C: document.getElementById('color-3').labels[0],
	};
	updatePallet()

	{	const id = 'sidebar-rake-placement'
		const el = document.getElementById(id);
		fetchAndSet(el, id)
		rake.config.placement = new RakeConfig(el.value);
		el.addEventListener('change', (ev)=>{
			try {
				rake.config.placement = new RakeConfig(el.value);
				storage.store(id, el.value);
			} catch (e) {
				// TODO: error handling
			}
		});
		el.addEventListener("keydown", (ev)=>{if (ev.which == 13) {el.blur();}});
	}
	backend.dom_setup = true;
	init();
});


function drawline(ctx, start, end) {
	ctx.beginPath();
	ctx.moveTo(start.x, start.y);
	ctx.lineTo(end.x,end.y);
	ctx.stroke();
};

var dropper = {
	circle: null,
	active: false,
	speed: 0.00002,
	fnSwitch: 100, // time to switch between linear and sqrt
	slope: null, // sqrt(fnSwitch * dropper.speed) / fnSwitch
	slowF: 3,
	time: null,
	lastdrop: null,
	draw: function(canvas, x,y) {
		const size = tool.translate({x: 32, y: 32});
		canvas.drawImage(dropper.img,x,y-size.y,size.x,size.y);
	},
	drop: function(time) {
			if(dropper.slope === null) { dropper.slope = Math.sqrt(dropper.fnSwitch * dropper.speed) / dropper.fnSwitch}
			if (dropper.active) {
				if (!dropper.time) {
					dropper.time = time;
				}
				var r = Math.sqrt((time - dropper.time - dropper.fnSwitch * (dropper.slowF-1)) * dropper.speed);
				if(time - dropper.time < dropper.fnSwitch * dropper.slowF ) {
					r = (time - dropper.time) * dropper.slope / dropper.slowF
				}
				backend.resizeDrop(dropper.circle, r);
				window.requestAnimationFrame(dropper.drop);
			} else {
				backend.finishDrop(dropper.drop);
			}
		},
	down: function(x,y,w,h) {
		var cId = 0;
		switch (color) {
			case 'A': cId = 0; break;
			case 'B': cId = 1; break;
			case 'C': cId = 2; break;
		}
		const circle = {x: 2*x/w - 1, y: 1 - 2 * y / h, r: 0.0};
		dropper.circle = backend.addDrop(circle.x, circle.y, circle.r, cId);

		dropper.active = true;
		window.requestAnimationFrame(dropper.drop);
	},
	up: function() {
		dropper.active = false;
		dropper.time = null;
	}

};

var sparkle_dropper = {
	draw: function(canvas, x,y) {
		const size = tool.translate({x: 32, y: 32});
		canvas.drawImage(sparkle_dropper.img,x-size.x/2,y-size.y/2,size.x,size.y);
		// canvas.drawImage(sparkle_dropper.img,y-size/2,x-size/2,size.x,size.y);
	}
};

// based on: https://gist.github.com/jwir3/d797037d2e1bf78a9b04838d73436197
function drawArrow(context, from, to, radius) {
	var x_center = to.x;
	var y_center = to.y;

	var angle;
	var x;
	var y;

	context.beginPath();
	context.moveTo(from.x, from.y);
	context.lineTo(to.x,to.y);
	context.stroke();
	context.beginPath();

	angle = Math.atan2(to.y - from.y, to.x - from.x)
	x = radius * Math.cos(angle) + x_center;
	y = radius * Math.sin(angle) + y_center;

	context.moveTo(x, y);

	angle += (1.0/3.0) * (2 * Math.PI)
	x = radius * Math.cos(angle) + x_center;
	y = radius * Math.sin(angle) + y_center;

	context.lineTo(x, y);

	angle += (1.0/3.0) * (2 * Math.PI)
	x = radius *Math.cos(angle) + x_center;
	y = radius *Math.sin(angle) + y_center;

	context.lineTo(x, y);

	context.closePath();

	context.fill();
}

var rake_dropper = {
	canvas: undefined,
	ctx: undefined,
	config: {
		w: 30,
		h: 30,
		of: 15,
	},
	init: function() {
		if (!rake_dropper.canvas && !rake_dropper.ctx) {
			rake_dropper.canvas = document.createElement('canvas');
			rake_dropper.canvas.width = 32;
			rake_dropper.canvas.height = 32;
			rake_dropper.ctx = rake_dropper.canvas.getContext('2d');
		}
	},
	setPattern: function(ctx) {
		if(this.config_last === undefined) { this.config_last = {}; }
		var diffs = false;
		Object.keys(rake_dropper.config).forEach(function(key){
			if (rake_dropper.config_last[key] !== rake_dropper.config[key]) {
				diffs = true;
				rake_dropper.config_last[key] = rake_dropper.config[key];
			}
		});
		
		if (diffs) {
			rake_dropper.init();

			const w = ctx.canvas.width * this.config.w / 1000;
			const h = ctx.canvas.height * this.config.h / 1000;
			const of =(ctx.canvas.height * this.config.of / 1000) % h;
			const r = 5;
			this.size = {x: r, y: r};
			this.pattern_size = {x: w * 2, y: h};
			const pattern_offset = {x: w, y: of};

			rake_dropper.canvas.width = this.pattern_size.x;
			rake_dropper.canvas.height = this.pattern_size.y;
			rake_dropper.ctx.beginPath();
			rake_dropper.ctx.ellipse(this.size.x, this.size.y,this.size.x,this.size.y,0, 2 * Math.PI, false);
			rake_dropper.ctx.moveTo(this.size.x+pattern_offset.x + this.size.x, this.size.y+pattern_offset.y);
			rake_dropper.ctx.ellipse(this.size.x +pattern_offset.x,this.size.y+pattern_offset.y,this.size.x,this.size.y,0, 2 * Math.PI, false);
			rake_dropper.ctx.stroke();
			this.pattern = ctx.createPattern(rake_dropper.canvas, 'repeat');
		}

		ctx.fillStyle = this.pattern;
		return {size: this.size, pattern_size: this.pattern_size};
	},
	draw: function(ctx, x, y, w, h) {
		const {size, pattern_size} = rake_dropper.setPattern(ctx);

		x = x % pattern_size.x;
		y = y % pattern_size.y;
		ctx.translate(x - size.x,y - size.y);
		ctx.fillRect(-x,-y,w+x,h+y);
		ctx.setTransform(1,0,0,1,0,0);
	},
};

var rake = {
	borderPoint: function(a, m, w, h, x) {
		const y = a + m * x;
		if(y < 0) {
			return {y: 0, x: (0-a)/m};
		} else if (y >= h) {
			return {y: h, x: (h-a)/m};
		} else {
			return {y, x};
		}
	},
	lrBound: function(start, end , w, h) {
		const v = {x: end.x - start.x, y: end.y - start.y};
		const m = v.y / v.x;
		if(Math.abs(m) === Infinity) {
			const leftBound = {x: start.x, y: 0};
			const rightBound = {x: start.x, y: h};
			return { leftBound, rightBound};
		}
		const a = start.y - m * start.x;

		const leftBound = rake.borderPoint(a, m,w,h, 0);
		const rightBound = rake.borderPoint(a, m, w, h, w);
		return { leftBound, rightBound};
	},
	straight: function(ctx, start, end, w, h) {
		const {leftBound, rightBound} = rake.lrBound(start, end, w, h);

		ctx.lineWidth = 0.5;
		ctx.beginPath();
		ctx.moveTo(leftBound.x, leftBound.y);
		ctx.lineTo(rightBound.x, rightBound.y);
		ctx.stroke();

		ctx.lineWidth = 2;
		ctx.strokeStyle = 'black';
		ctx.fillStyle = 'black';
		drawArrow(ctx, start, end, 10);
	},
	curve: function(ctx, start, end, w, h) {
		const {leftBound, rightBound} = rake.lrBound(start, end, w, h);
		var a;
		var startBound;
		if (end.x - start.x > 0 || (end.x === start.x && start.y < end.y)) {
			a = {x: rightBound.x - leftBound.x, y: rightBound.y - leftBound.y};
			startBound = leftBound;
		} else {
			a = {x: leftBound.x - rightBound.x, y: leftBound.y - rightBound.y};
			startBound = rightBound;
		}
		const len = Math.sqrt(a.x*a.x + a.y*a.y);
		const phaseOff = Math.sqrt((start.x - startBound.x)**2 + (start.y - startBound.y)**2);
		const up = {x: -a.y / len, y: a.x / len};

		ctx.lineWidth = 0.5;
		ctx.beginPath();
		const step = 20;
		{
			const i = - rake.config.periode / 4 - step;
			const mag = Math.sin((i-phaseOff) / rake.config.periode * Math.PI) * rake.config.magnitude;
			const x = startBound.x + i / len * a.x;
			const y = startBound.y + i / len * a.y;
			ctx.moveTo(x + up.x * mag, y + up.y * mag);
		}
		for(i = -rake.config.periode/4; i <= len + rake.config.periode / 4 + step; i += step) {
			const x = startBound.x + i / len * a.x;
			const y = startBound.y + i / len * a.y;
			const mag = Math.sin((i-phaseOff) / rake.config.periode * Math.PI) * rake.config.magnitude;
			ctx.lineTo(x + up.x * mag ,y + up.y * mag);
		}
		ctx.stroke();

		ctx.lineWidth = 2;
		drawArrow(ctx, start, end, 10);
	},
	config: {of: 5, line: function(a,b,c,d,e) {rake.straight(a,b,c,d,e);}},
	init: function() {
		if (! rake.canvas && ! rake.ctx) {
			rake.canvas = document.createElement('canvas');
			rake.canvas.width = 32;
			rake.canvas.height = 32;
			rake.ctx = rake.canvas.getContext('2d');
		}
	},
	setPattern: function(ctx) {
		if(this.placement !== rake.config.placement) {
			console.log(rake.config.placement)
			this.placement = rake.config.placement;
			rake.init();
			const r = 5;
			this.size = {x: r, y: r};
			const scale = ctx.canvas.width / 100
			const of = this.placement.patternWidth * scale
			rake.canvas.width = of;
			rake.canvas.height = this.size.y * 2;
			rake.ctx.setTransform(1,0,0,1,0,0);
			rake.ctx.beginPath();
			{
				var x = r
				const y = r
				rake.config.placement.config.spacing.forEach( function(space) {
					rake.ctx.moveTo(x + r,y)
					rake.ctx.ellipse(x, y, r, r, 0, 2 * Math.PI, false)
					x += (1+space) * scale
				})
			}
			rake.ctx.stroke();
			this.pattern = ctx.createPattern(rake.canvas, 'repeat');
		}
		ctx.strokeStyle = this.pattern;
		return this.size.x;
	},
	onemove: function(ctx, start, end, w, h) {

		rake.setPattern(ctx);
		const a = {y: -end.x + start.x, x: end.y - start.y};
		const r = rake.setPattern(ctx);
		const rot = Math.atan2(a.y,a.x);
		const {leftBound, rightBound} = rake.lrBound(start, {x: start.x + a.x, y: start.y + a.y}, w, h)
		const len = Math.sqrt((leftBound.x - rightBound.x)**2 + (leftBound.y - rightBound.y)**2);
		const startBound = a.x > 0 || (a.x === 0 && a.y > 0) ? leftBound : rightBound;

		const scale = ctx.canvas.width / 100
		const d = Math.sqrt((start.x - startBound.x)**2 + (start.y - startBound.y)**2)
		const offset = (- (d == NaN ? 0 : d) + this.placement.config.origin * scale) % rake.canvas.width;
		ctx.lineWidth = 2*r;
		ctx.beginPath();
		ctx.translate(startBound.x, startBound.y);
		ctx.rotate(rot);
		ctx.translate(-r - offset,-r);
		ctx.moveTo(0 + offset,r);
		ctx.lineTo(len + offset,r)
		ctx.stroke();

		ctx.setTransform(1,0,0,1,0,0);

		ctx.strokeStyle = 'black';
		rake.config.line(ctx, start,end, w, h);
	},
	up: function(start, end, w, h) {
		if(!start || !end) { return}
		const d = {x: end.x - start.x, y: end.y - start.y} 
		const len =  Math.sqrt(d.x*d.x+d.y*d.y)
		const up = {x: -d.y, y: d.x};
		var handle = 0;
		// TODO: adopt for free form rakes!
		if(Math.abs(d.y) > Math.abs(d.x)) {
			handle = Math.floor((start.y + 1.) / 2. * 1000.)
		} else {
			handle = Math.floor((start.x + 1.) / 2. * 1000.)
		}
		backend.rakeLinear(d.x/w, d.y/h, len,rake.config.placement.getNails(handle))
	}
};
// snap line parallel to axisa
function snap(start,end) {
	const a = {x: end.x - start.x, y: end.y - start.y};
	if(Math.abs(a.x) > Math.abs(a.y)) {
		a.y = 0;
	} else {
		a.x = 0;
	}
	return [start, {x: start.x + a.x, y: start.y + a.y}];
}

var tool = {
	start: null,
	overlay: null,
	tool: {
		draw: dropper,
		rake: rake,
	},
	leave: function() {
		tool.clear();
		if(tool.tool[state].up) {
			tool.tool[state].up();
		}
	},
	clear: function() {
		if(tool.ctx) {
			tool.ctx.clearRect(0,0,tool.overlay.width, tool.overlay.height);
		}
	},
	scale: function() {

		const rect = tool.overlay.getBoundingClientRect();
		return {x: tool.overlay.width / rect.width, y: tool.overlay.height / rect.height};
	},
	translate: function(pos) {
		const scale = tool.scale();
		return { x: pos.x * scale.x, y : pos.y * scale.y};
	},
	click: function(evnt) {
	},
	down: function(evnt) {
		tool.start = tool.translate({x: evnt.offsetX, y: evnt.offsetY});
		if (tool.tool[state].down) {
			tool.tool[state].down(tool.start.x, tool.start.y,
				tool.overlay.width, tool.overlay.height);
		}
	},
	over: function(evnt) {
		tool.pos = tool.translate({x: evnt.offsetX, y: evnt.offsetY})
		if (!tool.ctx) {
			tool.ctx = tool.overlay.getContext("2d");
		}
		if(tool.ctx) {
			tool.clear();
			const p = tool.pos;
			if(tool.start !== null && tool.tool[state].onemove) {
				const [s,e] = snap(tool.start, p);
				tool.tool[state].onemove(tool.ctx, s,e , tool.overlay.width, tool.overlay.height);
			} else if (tool.tool[state].draw) {
				tool.tool[state].draw(tool.ctx, p.x, p.y, tool.overlay.width, tool.overlay.height);
			}

		}
	},
	up: function(evnt) {
		if(tool.tool[state].up) {
			if(tool.start && tool.pos) {
				const [s,e]  = snap(tool.start, tool.pos)
				tool.tool[state].up(s,e,tool.overlay.width, tool.overlay.height);
			} else {
				tool.tool[state].up()
			}
		}
		tool.start = null;
		// tool.overlay.drawImage()
	},
};

