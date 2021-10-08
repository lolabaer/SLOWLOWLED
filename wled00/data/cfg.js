
import { lang } from './cfg_lang.js'

// minimal version of chibi - todo convert to module exporting '$' and remove from this file
(function () {
	'use strict';
	
	var d = document,
		w = window;

	// Loop through node array
	function nodeLoop(fn, nodes) {
		var i;
		// Good idea to walk up the DOM
		for (i = nodes.length - 1; i >= 0; i -= 1) {
			fn(nodes[i]);
		}
	}		

	function wled(selector) {
		var cb, nodes = [], json = false, nodelist, i;

		if (selector) {

			// Element node, would prefer to use (selector instanceof HTMLElement) but no IE support
			if (selector.nodeType && selector.nodeType === 1) {
				nodes = [selector]; // return element as node list
			} else if (typeof selector === 'object') {
				// JSON, document object or node list, would prefer to use (selector instanceof NodeList) but no IE support
				json = (typeof selector.length !== 'number');
				nodes = selector;
			} else if (typeof selector === 'string') {
				nodelist = d.querySelectorAll(selector);

				// Convert node list to array so results have full access to array methods
				// Array.prototype.slice.call not supported in IE < 9 and often slower than loop anyway
				for (i = 0; i < nodelist.length; i += 1) {
					nodes[i] = nodelist[i];
				}
			}
		}
		
		// Only attach nodes if not JSON
		cb = json ? {} : nodes;
		
		// Executes a function on nodes
		cb.each = function (fn) {
			if (typeof fn === 'function') {
				nodeLoop(function (elm) {
					// <= IE 8 loses scope so need to apply
					return fn.apply(elm, arguments);
				}, nodes);
			}
			return cb;
		};
		
		return cb;
	}

	// Set Chibi's global namespace here ($)
	w.$ = wled;

}());

function setLabel(elm) {
	const id = elm.id;
	const label = lang.labels[id];
	elm.textContent = label ? label : id;
}

//startup, called on page load
function S() {
  $('.l').each(setLabel); //populate labels
}

//toggle between hidden and 100% width (screen < ? px) 
//toggle between icons-only and 100% width (screen < ?? px)
//toggle between icons-only and ? px (screen >= ?? px)
function menu() {

}

S();