var active = {};
document.addEventListener("click", function(evnt){
	var el = evnt.target;
	if (el.tagName == 'INPUT'
		&& el.attributes.type
		&& el.attributes.type.nodeValue == 'radio')
	{
		if (active[el.attributes.name.nodeValue] == el) {
			var menu;
			console.log(el.parentNode.parentNode);
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
			if (menu) {
				menu.classList.toggle('hide');
			}
		} else {
			active[el.attributes.name.nodeValue] = el;
		}
	}
});
