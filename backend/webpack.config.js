module.exports = {
	module: {
		rules: [
			{
				test: /\.pegjs$/,
				use: 'pegjs-loader'
			}
		]
	},
	mode: "development",
	entry: {
		index: "/home/runner/work/marbellous/marbellous/backend/../frontend//src/index.js",
	},
	resolve: {
		modules: ['/home/runner/work/marbellous/marbellous/backend/node_modules', 'node_modules']
	},
}
