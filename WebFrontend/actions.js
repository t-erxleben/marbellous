var active = {};
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
});
