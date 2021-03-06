import * as parser from './rake_syntax.pegjs'
import * as png from 'fast-png'
import * as libSlider from 'nouislider'
import 'nouislider/dist/nouislider.css';

function createSlider(node, obj) {
	libSlider.create(node, obj)
	if(!(obj.start instanceof Array) || obj.start.length == 1) {
		var d = document.createElement('div')
		d.classList.add('dummy_handle')
		node.querySelector('.noUi-base').prepend(d)
	}
}

const int = parseInt;
var popup = {}
var inputs = {}
window.Storage =  class Storage {
	constructor() {
		const store = window.localStorage;
		if(store.getItem('store-active'))	{
			this.fetch = function(id) {
				return store.getItem(id)
			}
			this.store = function(id, value) {
				inputs[id] = value
				store.setItem(id, value)
			}
			this.backup = function() {
				inputs = {}
				for(var i = 0; i < store.length; i++) {
					inputs[store.key(i)] = store.getItem(store.key(i))
				}
			}
			Object.keys(inputs).forEach(function(key){
				this.store(key, inputs[key])
			}, this)
		} else {
			this.fetch = function(id) { return inputs[id] }
			this.store = function(id, value) {
				inputs[id] = value
			}
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
			return [...new Array(1000).keys()]
				.map(function(i) {
					return i - (handle + this.config.origin * 10)}, this)
				.map(function(d) {
					if(Math.abs(d % 10) == 0) {
						d = Math.floor(d/10)
						if(d >= 0)  {
							return d % this.patternWidth
						} else {
							return this.patternWidth - 1 + ((d+1) % this.patternWidth)
						}
					} else {
						return -1
					}
				}, this)
				.map(function(i) { return i == -1 ? 0 : this.pattern[i]}, this)
		}
	}
}
function parseRadiusRange(str) {
	const range = {};
	range.min = int(str)
	const p = range.min == 0 ? 1 : Math.ceil(Math.log10(range.min) + Number.EPSILON)
	if(str[p] != '-') { throw "failed to parse!" }
	range.max = int(str.slice(p+1))
	if(range.max == 0) { throw "max to small!"}
	range.min /= 100.
	range.max /= 100.
	return range;
}

var active = {};
const states = ['draw','rake'];
var state = 'draw';
var nodes = {};
var backend = {fn_bind: false, dom_setup: false, init: false, filter: false, resolution: null};
var color = 0x228B22;
var pallets = { nodes: [] }
window.Module = {
	onRuntimeInitialized: function() {
		backend.addPallete = Module.cwrap('addPalette',
			'number', ['number']);
		backend.setActivePalette = Module.cwrap('setActivePalette',
			'number', ['number']);
		backend.setBGColor = Module.cwrap('setBGColor',
			'void', ['number']);
		backend.addDrop = Module.cwrap('addDrop',
			'void', ['number', 'number', 'number', 'number']);
		backend.addDrops = Module.cwrap('addDrops',
			'void', ['number', 'array']);
		backend.resizeDrops = Module.cwrap('resizeDrops',
			'number', ['number']);
		backend.setColorAt = Module.cwrap('setColorAt',
			'number', ['number', 'number']);
		backend.setColorRatioAt = Module.cwrap('setColorRatioAt',
			'number', ['number', 'number'])
		backend.redraw = Module.cwrap('redraw',
			'void', []);
		backend.startRaking = Module.cwrap('startRaking', 'void', [])
		backend.startDropping = Module.cwrap('startDropping', 'void', [])
		backend.rake_impl = Module.cwrap('rake',
			'void', [	'number', 'number',
						'number', 'number', 'number',
						'array'])
		backend.rake = function(a,b,c,d,e,f) {
			backend.rake_impl(a,b,c,d,e,f)
			document.getElementById('undo').disabled = false
		}
		backend.sprinklerGlobal = Module.cwrap('sprinkleGlobal', 'void', ['number', 'number', 'number'])
		backend.sprinklerLocal = Module.cwrap('sprinkleLocal', 'void', ['number', 'number', 'number', 'number', 'number', 'number'])
		backend.finishDrops = Module.cwrap('finishDrops', 'number', [])
		backend.clearCanvas = Module.cwrap('clearCanvas', 'void', [])
		backend.initBackend = Module.cwrap('initBackend', 'void', ['string', 'number', 'number'])
		backend.setFilter = Module.cwrap('setFilter', 'void', ['number'])
		backend.undoLastRake = Module.cwrap('undoLastRake', 'void', [])
		backend.resize = Module.cwrap('resize', 'void', ['number', 'number'])
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
	pallets[pallets.active].id = backend.addPallete(pallets.inputs.length);
	backend.setActivePalette(pallets[pallets.active].id);
	pallets[pallets.active].colors.forEach(function(x,i) {
		backend.setColorAt(i, color2int(x.color))
		backend.setColorRatioAt(i, int(x.ratio))
	})
	backend.initBackend('#image', backend.resolution, backend.resolution)
	backend.setBGColor(color2int(pallets[pallets.active].background));
	color = 0
	backend.setFilter(backend.filter ? 1 : 0)
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
	pallets.nodes.forEach(function(x,i){
		x.style.background = pallets[pallets.active].colors[i].color
	})
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

	if(_old === 'rake' ) { document.getElementById('undo').disabled = true }
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
	const label = document.getElementById('switch-state').parentNode.querySelector('label');
	const img = label.querySelector('img');
	switch(_new) {
		case 'rake':
			img.src = 'icons/fast-backward-solid.svg';
			label.title = 'Go back to color dropping.';
			backend.startRaking()
		break;
		case 'draw':
			label.title = 'Start raking.';
			img.src = 'icons/arrow-right-solid.svg';
			if(backend.init) { backend.startDropping() }
		break;
	}
}

var download_side = null;
function downloadCanvas(el) {
	const ptr = Module.ccall("getImage", "number", [], []);	
	const width = Module.HEAP32[ptr/4]
	const height = Module.HEAP32[ptr/4+1]
	const len = width * height * 3

	const data = Module.HEAPU8.slice(ptr + 8, ptr+len+8);
	const pngData = png.encode({width, height, data, depth: 8, channels: 3})
	const blob = new Blob([pngData], {type: 'image/png'});
	if(!download_side) {
		download_side = document.createElement('a');
		document.body.appendChild(download_side);
		download_side.style.display = 'none';
	}
	const url = window.URL.createObjectURL(blob);
	download_side.href = url;
	download_side.download = "marebllous-image.png";
	download_side.click();
	el.hidden = true;
}

function handleClick(el) {
		switch(el.id) {
			case 'help':
				popup.popup.classList.add('popup_open')
				break;
			case 'download':
				const spinner = el.parentElement.getElementsByClassName('spinner')[0]
				if(spinner.hidden) {
					spinner.hidden = false
					setTimeout(()=>downloadCanvas(spinner), 100)
				}
				break;
			case 'undo':
				backend.undoLastRake()
				el.disabled = true
				break;
			case 'clear':
				if(window.confirm('All your work will be lost.\nAre you sure?'))
				{
					backend.clearCanvas()
					// go back to draw when clearing in rake
					if(state == 'rake') {
						switchState('rake', 'draw'); state = 'draw'
					}
				}
				break;
			case 'color-dropper':
				tool.tool[state] = dropper;
				break;
			case 'color-rake':
				tool.tool[state] = rake_dropper;
				sidebar.options.colorgrid.checked = true
				break;
			case 'color-sprinkler':
				tool.tool[state] = sparkle_dropper;
				sidebar.options.sprinkler.checked = true
				break;
			case 'rake-movement-linear':
				rake.config.line = rake.straight;
				break;
			case 'rake-movement-wave':
				rake.config.line = rake.curve;
				break;
			case 'switch-state':
				const oldState = state;
				if (oldState === 'rake') {
					if(!window.confirm("Going back to color dropping will undo all rake steps.\nAre you sure?"))
					{
						break;
					}
				}
				switch(state) { case 'draw': state = 'rake'; break; case 'rake': state = 'draw'; break; }
				switchState(oldState, state);
				break;
			default:
				const cid = pallets.nodes.indexOf(el.labels[0])
				if (cid >= 0) { color = cid }
		}
}

document.addEventListener("click", function(evnt){
	var el = evnt.target;
	// hide submenu if element is double clicked, show it again when clicked again or first selected
	if (el.tagName == 'INPUT'
		&& el.attributes.type
		&& el.attributes.type.nodeValue == 'radio'
		&& !el.classList.contains('collapsible')
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
	alert("There is no support for touch input at the moment!");	
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
window.onload = function() {
	document.querySelectorAll('menu.sidebar > li > div').forEach(function(div){
		div.style.maxHeight = div.scrollHeight + 3 + 'px';
	});
	document.querySelectorAll('input.collapsible + label + div').forEach(function(div){
		div.style.maxHeight = div.scrollHeight + 3 + 'px';
	})
}

function DomInit(){
	storage = new Storage()
	document.querySelectorAll('menu input[type="radio"]').forEach(function(rad){
		if (rad.checked) {
			active[rad.attributes.name.nodeValue] = rad;
			handleClick(rad);
		} else if(active[rad.attributes.name.nodeValue] === undefined) {
			active[rad.attributes.name.nodeValue] = null;
		}
	});
	popup.popup = document.querySelector('.popup')
	popup.popup.addEventListener('click', function(ev){
		if(ev.target === ev.currentTarget) {
			popup.popup.classList.remove('popup_open')
		}
	})
	popup.close = popup.popup.querySelector('.close_button')
	popup.close.addEventListener('click', function() { popup.popup.classList.remove('popup_open') })
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

	sidebar.options = {
		rake: document.getElementById('sidebar-rake'),
		pallet: document.getElementById('sidebar-pallet'),
		sprinkler: document.getElementById('sidebar-sprinkler'),
	};
	sidebar.btn = document.getElementById('sidebar-btn');
	sidebar.menu = document.querySelector('menu.sidebar');
	{
		const id = 'sidebar-general-resolution'
		const el = document.getElementById(id)
		fetchAndSet(el, id)
		backend.resolution = int(el.value)
		el.addEventListener("change", (ev)=>{
			if(state != 'rake' || window.confirm("Changing the resolution will undo all rake steps!\nAre you sure?")) {
				backend.resolution = int(el.value)
				storage.store(id, el.value)
				backend.resize(backend.resolution, backend.resolution)
				if (state === 'rake') { switchState('rake', 'draw'); state = 'draw' }
			}
		})
	}
	{
		const id = 'sidebar-pallet-active-pallet'
		const el = document.getElementById(id);
		fetchAndSet(el, id);
		pallets.active = el.value;
		el.addEventListener("change", (ev)=>{pallets.active = el.value
			storage.store(id, el.value);
			if(pallets[pallets.active] === undefined) {
				pallets[pallets.active] = {id: backend.addPallete(pallets.inputs.length), colors: []};
			}
			backend.setActivePalette(pallets[pallets.active].id);

			var {colors, background} = pallets[pallets.active];
			
			pallets.inputs.forEach(function(x,i){
				if(!colors[i]) {
					fetchAndSet(x.color, pallets.active + 'sidebar-pallet-color-' + i.toString())
					fetchAndSet(x.ratio, pallets.active + 'sidebar-pallet-ratio-' + i.toString())
				} else {
					x.color.value = colors[i].color
					x.ratio.value = colors[i].ratio
				}
			})
			if(background == null) { fetchAndSet(pallets.inputs.background, pallets.active + 'sidebar-pallet-background') }
			else { pallets.inputs.background.value = background }


			pallets.inputs.forEach(function(x,i) {
				colors[i] = {
					color: x.color.value,
					ratio: x.ratio.value
				}
				storage.store(pallets.active + 'sidebar-pallet-color-' + i.toString(), x.color.value)
				storage.store(pallets.active + 'sidebar-pallet-ratio-' + i.toString(), x.ratio.value)
			});
			background = pallets.inputs.background.value;
			storage.store(pallets.active + 'sidebar-pallet-background', pallets.inputs.background.value)

			pallets[pallets.active] = {colors, background}
			pallets[pallets.active].colors.forEach(function(x,i){
				backend.setColorAt(i, color2int(x.color))
				backend.setColorRatioAt(i, int(x.ratio))
			})
			backend.setBGColor(color2int(background))
			backend.redraw();
			updatePallet();
		});
		pallets.inputs = [];
		if(pallets[pallets.active] == undefined) {
			pallets[pallets.active] = { colors: [] }
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
	var i = 0
	var sbc = null
	while(sbc = document.getElementById('sidebar-pallet-color-' + i.toString()))
	{
		const el = sbc
		const num = i
		const iid = 'sidebar-pallet-color-' + num.toString();
		const riid = 'sidebar-pallet-ratio-' + num.toString()
		const rel = document.getElementById(riid);
		const rdef = rel.value
		const id = function() { return pallets.active + iid }
		const rid = function() { return pallets.active + riid }
		fetchAndSet(el, id());
		fetchAndSet(rel, rid());
		pallets[pallets.active].colors[i] = {
			color: el.value,
			ratio: rel.value
		}
		el.addEventListener("change", (ev)=>{
			pallets[pallets.active].colors[num].color = el.value;
			storage.store(id(), el.value);
			backend.setColorAt(num, color2int(el.value));
			backend.redraw();
			updatePallet();});
		rel.addEventListener('change', (ev)=>{
			if(rel.value === "") { rel.value = rdef }
			pallets[pallets.active].colors[num].ratio = rel.value
			storage.store(rid(), rel.value)
			backend.setColorRatioAt(num, int(rel.value))
		})
		rel.addEventListener('keydown', (ev) => { if(ev.which === 13) { rel.blur() } })
		pallets.inputs[num] = {
			color: el,
			ratio: rel
		}
		pallets[pallets.active].colors[num] = {
			color: el.value,
			ratio: rel.value
		}
		i += 1
	}
	
	pallets.nodes = []
	pallets.inputs.forEach(function(x,i){
		pallets.nodes[i] = document.getElementById('color-' + i.toString()).labels[0]
	})
	updatePallet()

	{	const id = 'sidebar-rake-placement'
		const el = document.getElementById(id);
		const def = el.value
		fetchAndSet(el, id)
		rake.config.placement = new RakeConfig(el.value || def);
		el.addEventListener('change', (ev)=>{
			try {
				if(el.value === "") { el.value = def }
				rake.config.placement = new RakeConfig(el.value);
				storage.store(id, el.value);
			} catch (e) {
				// TODO: error handling
			}
		});
		el.addEventListener("keydown", (ev)=>{if (ev.which == 13) {el.blur();}});
	}
	{
		const id = 'sidebar-rake-periode'
		const el = document.getElementById(id)
		const slider = document.getElementById(id + '-slider')
		const def = el.value;
		fetchAndSet(el, id)
		rake.config.periode = int(el.value || def) / 100
		el.addEventListener('change', (ev)=>{
			try {
				if(el.value === "") { el.value = def }
				const v = int(el.value)
				rake.config.periode = v / 100
				slider.noUiSlider.set(v)
				storage.store(id, el.value)
			} catch(e) {}
		})
		el.addEventListener('keydown', (ev)=>{if (ev.which == 13) {el.blur()}})
		createSlider(slider,{
			start: rake.config.periode * 100,
			connect: [true, false],
			range: {
				min: int(el.min),
				max: int(el.max)
			}
		})
		slider.noUiSlider.on('update', function(vals) {
			el.value = `${Math.round(vals[0])}`
			rake.config.periode = vals[0] / 100.
			storage.store(id, el.value)
		})
	}
	{
		const id = 'sidebar-rake-magnitude'
		const el = document.getElementById(id)
		const slider = document.getElementById(id + '-slider')
		const def = el.value
		fetchAndSet(el, id)
		rake.config.magnitude = int(el.value || def) / 100
		el.addEventListener('change', (ev)=>{
			try {
				if(el.value === "") { el.value = def }
				const v = int(el.value)
				rake.config.magnitude = v / 100
				slider.noUiSlider.set(v)
				storage.store(id, el.value)
			} catch(e) {}
		})
		el.addEventListener('keydown', (ev)=>{if (ev.which == 13) {el.blur()}})
		createSlider(slider, {
			start: rake.config.magnitude * 100,
			connect: [true, false],
			step: 1,
			range: {
				min: int(el.min),
				max: int(el.max)
			},
		})
		slider.noUiSlider.on('update', function(vals) {
			el.value = `${Math.round(vals[0])}`
			rake.config.magnitude = vals[0] / 100.
			storage.store(id, el.value)
		})
	}
	{	const id = 'sidebar-sprinkler-radius'
		const el = document.getElementById(id)
		const slider = document.getElementById(id + '-slider')
		const def = el.value

		fetchAndSet(el, id)
		sparkle_dropper.range = parseRadiusRange(el.value || def);
		el.addEventListener('change', (ev)=>{
			try {
				if(el.value === "") { el.value = def }
				sparkle_dropper.range = parseRadiusRange(el.value)
				slider.noUiSlider.set([sparkle_dropper.range.min*100, sparkle_dropper.range.max*100])
				storage.store(id, el.value)
			} catch(e) {
				console.error("failed to parse radius range: ", e)
				// TODO: error handling!
			}
		})
		el.addEventListener("keydown", (ev)=>{if (ev.which == 13) {el.blur();}})

		createSlider(slider, {
				start: [sparkle_dropper.range.min * 100, sparkle_dropper.range.max * 100],
				connect: true,
				step: 1,
				range: {
					min: 1,
					max: 100
				}
			}
		)
		slider.noUiSlider.on('update', function(vals) {
			el.value = `${Math.round(vals[0])}-${Math.round(vals[1])}`
			sparkle_dropper.range = parseRadiusRange(el.value)
			storage.store(id, el.value)
		})
	}
	const sidebar_fn = function(obj, key, config, value){
		const id ="sidebar-colorgrid-" + config
		const el = document.getElementById(id)
		fetchAndSet(el, id)
		obj[key] = int(el.value || value)	
		el.addEventListener('change', (ev)=> {
			if(el.validity.valid) {
				try {
					obj[key] = int(el.value || value)
					storage.store(id, el.value)
				} catch(e) { console.error(e) }
			}
		})
		el.addEventListener("keydown", (ev)=>{if (ev.which == 13) {el.blur()}})
	}

	{	const id = 'sidebar-sprinkler-frequence'
		const el = document.getElementById(id)
		const slider = document.getElementById(id+'-slider')
		const def = el.value
		fetchAndSet(el, id)
		sparkle_dropper.rate = 1000. / int(el.value || def) 
		el.addEventListener('change', (ev)=> {
			if(el.value === "") { el.value = def }
			if (el.validity.valid) {
				try {
					const v = int(el.value)
					sparkle_dropper.rate = 1000. / v
					slider.noUiSlider.set(v)
					storage.store(id, el.value)
				}	catch(e) { /* TODO: error handling */ }
			}
		})
		el.addEventListener('keydown', (ev)=>{if (ev.which == 13) {el.blur();}})
		createSlider(slider, {
				start: 1000. / sparkle_dropper.rate,
				step: 1,
				connect: [true, false],
				range: {
					min: int(el.min),
					max: int(el.max)
				}
			}
		)
		slider.noUiSlider.on('update', function(vals) {
			el.value = `${Math.round(vals[0])}`
			sparkle_dropper.rate = 1000. / vals
			storage.store(id, el.value)
		})
	}
	{	const id = 'sidebar-sprinkler-local'
		const el = document.getElementById(id)
		fetchAndSet(el, id)
		const sigId = 'sidebar-sprinkler-sig'
		const sigSlider = document.getElementById(sigId + "-slider")
		const sigEl = document.getElementById(sigId)
		const sigDef = sigEl.value
		fetchAndSet(sigEl, sigId)

		el.checked = el.value === 'true'
		sparkle_dropper.local = el.checked
		sigEl.disabled = !el.checked
		if(!el.checked) {
			sigSlider.setAttribute('disabled', true)
		} else {
			sigSlider.removeAttribute('disabled')
		}
		el.addEventListener('change', (ev) => {
			sparkle_dropper.local = el.checked
			storage.store(id, el.checked)
			sigEl.disabled = !el.checked
			if(!el.checked) {
				sigSlider.setAttribute('disabled', true)
			} else {
				sigSlider.removeAttribute('disabled')
			}
		})
		sparkle_dropper.sig = int(sigEl.value || sigDef ) / 100.
		sigEl.addEventListener('change', (ev)=> {
			if(sigEl.value === "") { sigEl.value = sigDef }
			if (sigEl.validity.valid) {
				try {
					sparkle_dropper.sig = int(sigEl.value) / 100.
					sigSlider.noUiSlider.set(sparkle_dropper.sig * 100)
					storage.store(sigId, sigEl.value)
				}	catch(e) {
					// TODO: error handling
				}
			}
		})
		sigEl.addEventListener('keydown', (ev)=>{if (ev.which == 13) {sigEl.blur();}})
		
		createSlider(sigSlider, {
			start: sparkle_dropper.sig * 100,
			connect: [true, false],
			step: 1,
			range: {
				min: int(sigEl.min),
				max: int(sigEl.max)
			}
		})
		sigSlider.noUiSlider.on('update', function(vals) {
			sigEl.value = `${Math.round(vals[0])}`
			sparkle_dropper.sig = int(sigEl.value) / 100.
			storage.store(sigId, sigEl.value)
		})
	}
	{
		const id = "sidebar-filter"
		const el = document.getElementById(id)
		fetchAndSet(el, id)
		el.checked = el.value === 'true'
		backend.filter = el.checked
		el.addEventListener("change", (ev)=>{
			storage.store(id, el.checked)
			backend.filter = el.checked
			backend.setFilter(backend.filter ? 1 : 0)
			backend.redraw()
		})

	}
	backend.dom_setup = true;
	init();
}
if(document.readyState === 'loading') {
	document.addEventListener("DOMContentLoaded", DomInit);
} else {
	DomInit()
}


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
				backend.resizeDrops(r);
				window.requestAnimationFrame(dropper.drop);
			} else {
				backend.finishDrops();
			}
		},
	down: function(x,y,w,h) {
		const circle = {x: 2*x/w - 1, y: 1 - 2 * y / h, r: 0.0};
		backend.addDrop(circle.x, circle.y, circle.r, color);

		dropper.active = true;
		window.requestAnimationFrame(dropper.drop);
	},
	up: function() {
		dropper.active = false;
		dropper.time = null;
	}

};

var sparkle_dropper = {
	rate: 50,
	draw: function(canvas, x,y, w, h) {
		sparkle_dropper.pos =  {x: x / w, y: y/h}
		sparkle_dropper.dim = {w, h}
		const size = tool.translate({x: 32, y: 32});
		canvas.drawImage(sparkle_dropper.img,x-size.x/2,y-size.y/2,size.x,size.y);
	},
	drop: function(time) {
		if(! sparkle_dropper.active) { sparkle_dropper.time = null; return }
		if(!sparkle_dropper.time) {
			sparkle_dropper.time = time;
			window.requestAnimationFrame(sparkle_dropper.drop);
			return }
		const d = time - sparkle_dropper.time
		const n = int(d / sparkle_dropper.rate)
		if(n > 0) {
			if(sparkle_dropper.local === true) {
				backend.sprinklerLocal(n, sparkle_dropper.range.min, sparkle_dropper.range.max,
					sparkle_dropper.pos.x * 2 - 1,1 - sparkle_dropper.pos.y * 2,
					sparkle_dropper.sig)
			} else {
				backend.sprinklerGlobal(n, sparkle_dropper.range.min, sparkle_dropper.range.max)
			}
			sparkle_dropper.time = time - (d - (n * sparkle_dropper.rate))
		}
		window.requestAnimationFrame(sparkle_dropper.drop)
	},
	down: function(x,y,w,h) {
		sparkle_dropper.pos = {
			x: x / w,
			y: y /h
		}
		sparkle_dropper.dim = {w, h}
		sparkle_dropper.active = true;
		window.requestAnimationFrame(sparkle_dropper.drop)
	},
	up: function() {
		sparkle_dropper.active = false;
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
		of: 10,
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

			const w = ctx.canvas.width * this.config.w / 100;
			const h = ctx.canvas.height * this.config.h / 100;
			const of =(ctx.canvas.height * this.config.of / 100) % h;
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
	down: function(x,y,w,h) {
		if(dropper.active) { return }
		const p = {x: x/w*2, y: y/h*2}
		const dim = {w: rake_dropper.config.w / 50, h: rake_dropper.config.h / 50,
			of: rake_dropper.config.of / 50}
		const max_w = Math.ceil(2/dim.w)
		const max_h = Math.ceil(2/dim.h) + 3
		const max_count =  max_w * max_h 

		if (max_count > 100) { console.error('Max count exceeds 100'); return }
		let count = 0
		const data = new Float32Array(max_count * 4)
		const space = {
			l: Math.floor(p.x / dim.w),
			r: Math.floor((2 - p.x) / dim.w),
			t: Math.ceil(p.y / dim.h - dim.of/dim.h),
			b: Math.floor((2 - p.y) / dim.h)
		}

		for(let i = -space.l; i <= space.r; i += 1) {
			for(let j = -space.t; j <= space.b; j += 1) {
				data[count*4] = (i * dim.w + p.x) - 1
				data[count*4 + 1] = 1 - (j * dim.h + p.y + Math.abs(i%2) * dim.of)
				data[count*4 + 2] = 0
				data[count*4 + 3] = rake_dropper.config.random_color ? -1 : color
				count += 1
			}
		}
		if(count > 100 || count > max_count) { console.error("Failed to set drops for grid(to many drops on screen!)") }
		backend.addDrops(count, new Int8Array(data.buffer))

		dropper.active = true
		window.requestAnimationFrame(dropper.drop)
	},
	up: function() {
		dropper.time = null;
		dropper.active = false;
	}
};

var rake = {
	scale: 2.,
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
		if(Math.abs(v.x) < Number.EPSILON && Math.abs(v.y) < Number.EPSILON) { return { leftBound: null, rightBound: null} }
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
		if(leftBound === null || rightBound === null) { return }

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
		if(leftBound === null || rightBound === null) { return }
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
		const step = (len * rake.config.periode) / 50
		{
			const i = - rake.config.periode / 4 - step;
			const mag = Math.sin((i-phaseOff) / (rake.config.periode * w) * Math.PI) * rake.config.magnitude * w;
			const x = startBound.x + i / len * a.x;
			const y = startBound.y + i / len * a.y;
			ctx.moveTo(x + up.x * mag, y + up.y * mag);
		}
		for(i = 0; i <= len + step; i += step) {
			const x = startBound.x + i / len * a.x;
			const y = startBound.y + i / len * a.y;
			const mag = Math.sin((i-phaseOff) / (rake.config.periode * w) * 2 * Math.PI) * rake.config.magnitude * w;
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
		if(leftBound === null || rightBound === null) { return }
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
		clamp(start,end,w,h,rake.scale)
		rake.config.line(ctx, start,end, w, h);
	},
	up: function(start, end, w, h) {
		if(!start || !end) { return}
		clamp(start, end, w,h, rake.scale)
		const d = {x: end.x - start.x, y: end.y - start.y} 
		const up = {x: -d.y, y: d.x};
		var handle = 0;
		// TODO: adopt for free form rakes!
		if(Math.abs(d.x) > Math.abs(d.y)) {
			handle = Math.floor(start.y / h * 1000.)
		} else {
			handle = Math.floor(start.x / w * 1000.)
		}
		if(rake.config.line === rake.straight) {
			backend.rake(
				d.x/w*rake.scale, d.y/h*rake.scale, // direction
				1, 0, 0,  // wave descritpion <- flat line
				rake.config.placement.getNails(handle) // pattern
			)
		} else if (rake.config.line === rake.curve) {
			const {leftBound, rightBound} = rake.lrBound(start, end, w, h)
			if(leftBound=== null || rightBound === null) { return }
			var offset;
			if (end.x - start.x > 0 || (end.x === start.x && start.y < end.y)) 
			{
				const startBound = leftBound;
				offset = Math.sqrt(
					(start.x - startBound.x)**2
					+ (start.y - startBound.y)**2) / w
			} else {
				const startBound = rightBound;
				offset = 1. - Math.sqrt(
					(start.x - startBound.x)**2
					+ (start.y - startBound.y)**2) / w
			}
			if(w != h) { console.error('only works on squares!') }
			backend.rake(
				d.x/w*rake.scale, d.y/h*rake.scale, // direction
				rake.config.periode,
				rake.config.magnitude,
				d.x != 0 ? offset : 1. -offset,
				rake.config.placement.getNails(handle) // rake pattern
			)
		} else {
			console.error('Failed to find correct rake type!')
		}
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

// clamp end to ||(end-start)/dim*scale|| <= 1
function clamp(start,end,w,h,scale) {
		const d = {x: end.x - start.x, y: end.y - start.y};
		const len = Math.sqrt((d.x / w * scale)**2 + (d.y / h * scale)**2);
		if( len > 1) {
			end.x = start.x + d.x / len;
			end.y = start.y + d.y / len;
		}
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
		tool.start = null;
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
	},
};

