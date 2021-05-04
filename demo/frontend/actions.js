var active = {};
const states = ['draw','rake'];
var state = 'draw';
var nodes = {};
var backend = {};
var color = 0x228B22;
var Module = {
	onRuntimeInitialized: function() {
		backend.dropColor = Module.cwrap('dropColor',
			'void',
			['number','number','number','number']);
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

}


function handleClick(el) {
		switch(el.id) {
			case 'color-1':
				color = 0x228B22;
				break;
			case 'color-2':
				color = 0xFFD700;
				break;
			case 'color-3':
				color = 0xDC143C;
				break;
			case 'color-dropper':
				tool.tool[state] = dropper;
				break;
			case 'color-rake':
				tool.tool[state] = rake_dropper;
				break;
			case 'color-rake-displaced':
				rake_dropper.config = {
					w: 30,
					h: 30,
					of: 15,
				};
				break;
			case 'color-rake-square':
				rake_dropper.config = {
					w: 30,
					h: 30,
					of: 0,
				};
				break;
			case 'color-rake-custom':
				rake_dropper.config = {
					w: 30,
					h: 15,
					of: 0,
				};
				console.warn('No custom input implemented!');
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
						img.src = 'icons/image-regular.svg';
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
		&& el.attributes.type.nodeValue == 'radio')
	{
		handleClick(el);
		const menu = getTopSubmenu(el);
		if (active[el.attributes.name.nodeValue] == el) {
			if (menu) {
				menu.querySelectorAll('li').forEach(function(li){li.classList.toggle('hide')});
			}
		} else {
			active[el.attributes.name.nodeValue] = el;
			if (menu) {
				menu.querySelectorAll('li').forEach(function(li){li.classList.remove('hide')});
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

document.addEventListener("DOMContentLoaded", function(){
	document.querySelectorAll('menu input[type="radio"]').forEach(function(rad){
		if (rad.checked) {
			active[rad.attributes.name.nodeValue] = rad;
		} else if(active[rad.attributes.name.nodeValue] === undefined) {
			active[rad.attributes.name.nodeValue] = null;
		}
	});
	tool.overlay = document.getElementById('overlay');
	tool.overlay.addEventListener("mousedown", function(evnt){
		hideMenu();
		tool.click(evnt);
	});
	tool.overlay.addEventListener("mousemove", tool.over);
	tool.overlay.addEventListener("mouseup", tool.up);
	tool.overlay.addEventListener("mousedown", tool.down);
	tool.overlay.addEventListener("mouseleave", tool.clear);

	states.forEach(function(s){
		switchState(s, null);
	});
	switchState(null, state);
	dropper.img = document.getElementById('img-color-dropper');
	sparkle_dropper.img = document.getElementById('img-color-sparkle');
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
	speed: 0.0002,
	time: null,
	draw: function(canvas, x,y) {
		const size = tool.translate({x: 32, y: 32});
		canvas.drawImage(dropper.img,x,y-size.y,size.x,size.y);
	},
	drop: function(time) {
			if (dropper.active) {
				if (!dropper.time) {
					dropper.time = time;
				}
				const r = (time - dropper.time) * dropper.speed;
				const arg = dropper.circle;
				backend.dropColor(arg.x, arg.y, r, arg.color);
				window.requestAnimationFrame(dropper.drop);
			}
		},
	down: function(x,y,w,h) {
		dropper.circle = {x: 2*x/w - 1, y: 1 - 2 * y / h, r: 0.0, color};
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
		rake_dropper.init();

		const r = 5;
		const size = tool.translate({x: r, y: r});
		const pattern_size = tool.translate({x: rake_dropper.config.w * 2, y: rake_dropper.config.h});
		const pattern_offset = tool.translate({x: rake_dropper.config.w, y: rake_dropper.config.of});

		rake_dropper.canvas.width = pattern_size.x;
		rake_dropper.canvas.height = pattern_size.y;
		rake_dropper.ctx.beginPath();
		rake_dropper.ctx.ellipse(size.x, size.y,size.x,size.y,0, 2 * Math.PI, false);
		rake_dropper.ctx.moveTo(size.x+pattern_offset.x + size.x, size.y+pattern_offset.y);
		rake_dropper.ctx.ellipse(size.x +pattern_offset.x,size.y+pattern_offset.y,size.x,size.y,0, 2 * Math.PI, false);
		rake_dropper.ctx.stroke();
		const pattern = ctx.createPattern(rake_dropper.canvas, 'repeat');
		ctx.fillStyle = pattern;
		return {size, pattern_size};
	},
	draw: function(ctx, x, y, w, h) {
		// TODO: only call when changed
		const {size, pattern_size} = rake_dropper.setPattern(ctx);

		x = x % pattern_size.x;
		y = y % pattern_size.y;
		ctx.translate(x - size.x,y - size.y);
		ctx.fillRect(-x,-y,w+x,h+y);
		ctx.setTransform(1,0,0,1,0,0);
	},
};

var rake = {
	straight: function(ctx, start, end) {
		ctx.beginPath();
		ctx.moveTo(start.x,start.y);
		ctx.lineTo(end.x, end.y);
		ctx.stroke();
	},
	curve: function(ctx, start, end) {
		const a = {x: end.x - start.x, y: end.y - start.y};
		const len = Math.sqrt(a.x*a.x + a.y*a.y);
		const up = {x: -a.y / len, y: a.x / len};
		ctx.beginPath();
		ctx.moveTo(start.x, start.y);
		var x = start.x;
		var y = start.y;
		const step = 20;
		for(i = step; i < len; i += step) {
			const mag = Math.sin(i / rake.config.periode * Math.PI) * rake.config.magnitude;
			ctx.lineTo(x + up.x * mag ,y + up.y * mag);
			x = start.x + i / len * a.x;
			y = start.y + i / len * a.y;
		}
		ctx.stroke();
	},
	config: {of: 5, line: function(a,b,c) {rake.straight(a,b,c);}},
	init: function() {
		if (! rake.canvas && ! rake.ctx) {
			rake.canvas = document.createElement('canvas');
			rake.canvas.width = 32;
			rake.canvas.height = 32;
			rake.ctx = rake.canvas.getContext('2d');
		}
	},
	setPattern: function(ctx) {
		rake.init();
		const r = 5;
		const size = tool.translate({x: r, y: r});
		rake.canvas.width = size.x * 2;
		rake.canvas.height = size.y * 4 + rake.config.of;
		rake.ctx.setTransform(1,0,0,1,0,0);
		rake.ctx.beginPath();
		rake.ctx.ellipse(size.x, size.y,size.x,size.y,0, 2 * Math.PI, false);
		rake.ctx.stroke();
		const pattern = ctx.createPattern(rake.canvas, 'repeat');
		ctx.strokeStyle = pattern;
		return size.x;
	},
	onemove: function(ctx, start, end, w, h) {
		const a = {y: -end.x + start.x, x: end.y - start.y};
		const r = rake.setPattern(ctx);
		const rot = Math.atan2(a.y,a.x);
		const f = a.x < a.y ? w / a.x : h / a.y;
		const p = a.x < a.y ? start.x / a.x : start.y / a.y;

		ctx.beginPath();
		ctx.moveTo(start.x - a.x * p, start.y - a.y * p);
		ctx.lineTo(start.x + a.x * (f-p), start.y + a.y *(f-p));
		ctx.lineWidth = r*2;
		ctx.translate(0,0);
		ctx.rotate(rot + Math.PI/2);
		ctx.stroke();
		ctx.setTransform(1,0,0,1,0,0);

		ctx.lineWidth = 2;
		ctx.strokeStyle = 'black';
		rake.config.line(ctx, start, end);
	}
};

var tool = {
	start: null,
	overlay: null,
	tool: {
		draw: dropper,
		rake: rake,
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
		if (!tool.ctx) {
			tool.ctx = tool.overlay.getContext("2d");
		}
		if(tool.ctx) {
			tool.clear();
			const p = tool.translate({x:evnt.offsetX, y:evnt.offsetY});
			if(tool.start !== null && tool.tool[state].onemove) {
				tool.tool[state].onemove(tool.ctx, tool.start, p, tool.overlay.width, tool.overlay.height);
			} else if (tool.tool[state].draw) {
				tool.tool[state].draw(tool.ctx, p.x, p.y, tool.overlay.width, tool.overlay.height);
			}

		}
	},
	up: function(evnt) {
		tool.start = null;
		if(tool.tool[state].up) {
			tool.tool[state].up();
		}
		// tool.overlay.drawImage()
	},
};

