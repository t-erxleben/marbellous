var active = {};
const states = ['draw','rake'];
var state = 'draw';
var nodes = {};

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
			case 'color-dropper':
				tool.tool = dropper;
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
	tool.overlay.addEventListener("click", function(evnt){
		hideMenu();
		tool.click(evnt);
	});
	tool.overlay.addEventListener("mousemove", tool.over);
	tool.overlay.addEventListener("mouseup", tool.up);
	tool.overlay.addEventListener("mousedown", tool.down);

	states.forEach(function(s){
		switchState(s, null);
	});
	switchState(null, state);
	dropper.img = document.getElementById('img-color-dropper');
});


function drawline(ctx, start, end) {
	ctx.beginPath();
	ctx.moveTo(start.x, start.y);
	ctx.lineTo(end.x,end.y);
	ctx.stroke();
}
var dropper = {
	draw: function(canvas, x,y) {
		const size = tool.translate({x: 30, y: 30});
		canvas.drawImage(dropper.img,x,y-size.y,size.x,size.y);
	}
};
function draw_rak(ctx, x, y, w, h) {}

var rak_dropper = {
	draw: draw_rak,
};
var line = {
	draw: draw_rak,
	onmove: drawline,
};
var tool = {
	start: null,
	overlay: null,
	tool: dropper,
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
	},
	over: function(evnt) {
		if (!tool.ctx) {
			tool.ctx = tool.overlay.getContext("2d");
		}
		if(tool.ctx) {
			tool.ctx.clearRect(0,0,tool.overlay.width, tool.overlay.height);
			const p = tool.translate({x:evnt.offsetX, y:evnt.offsetY});
			if(tool.tool.draw) {
				tool.tool.draw(tool.ctx, p.x, p.y, tool.overlay.width, tool.overlay.height);
			}
			if(tool.start !== null && tool.tool.onemove) {
				tool.tool.onmove(tool.ctx, tool.start, p);
			}
		}
	},
	up: function(evnt) {
		tool.start = null;
		// tool.overlay.drawImage()
	},
};
