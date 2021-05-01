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
			console.log(hit, next);
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



document.addEventListener("click", function(evnt){
	var el = evnt.target;
	// hide submenu if element is double clicked, show it again when clicked again or first selected
	if (el.tagName == 'INPUT'
		&& el.attributes.type
		&& el.attributes.type.nodeValue == 'radio')
	{
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
		switch(el.id) {
			case 'switch-state':
				console.log('switch');
				const img = el.parentNode.querySelector('img');
				const oldState = state;
				switch(state) {
					case 'draw': state = 'rake'; img.src = 'icons/image-regular.svg'; break;
					case 'rake': state = 'draw'; img.src = 'icons/arrow-right-solid.svg'; break;
				}
				switchState(oldState, state);
		}
	}
});

document.addEventListener("DOMContentLoaded", function(){
	document.querySelectorAll('menu input[type="radio"]').forEach(function(rad){
		active[rad.attributes.name.nodeValue] = null;
	});
	document.getElementById('overlay').addEventListener("click", function(){
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
	});
	states.forEach(function(s){
		switchState(s, null);
	});
	switchState(null, state);
});
