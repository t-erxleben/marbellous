module.exports = {
	module: {
		rules: [
			{
				test: /\.pegjs$/,
				use: 'pegjs-loader'
			}
		]
	},
	mode: "production",
	entry: {
		index: "/home/runner/work/marbellous/marbellous/backend/../frontend//src/index.js"
	},
}
