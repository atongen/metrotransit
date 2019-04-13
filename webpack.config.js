const path = require("path");

const isProd = process.env.NODE_ENV === "production";
const webpack = require("webpack");
const CompressionPlugin = require("compression-webpack-plugin");
const ManifestPlugin = require("webpack-manifest-plugin");
const UglifyJsPlugin = require("uglifyjs-webpack-plugin");
const HtmlWebpackPlugin = require("html-webpack-plugin");

module.exports = {
    plugins: [
        new webpack.HashedModuleIdsPlugin(),
        new CompressionPlugin({
            test: /\.js(\?.*)?$/i
        }),
        new ManifestPlugin({}),
        new HtmlWebpackPlugin({
            template: "./src/index.html.ejs",
            filename: "../index.html"
        })
    ],
    entry: {
        Index: "./lib/js/src/Index.js",
    },
    mode: isProd ? "production" : "development",
    output: {
        path: path.join(__dirname, "public/assets"),
        filename: isProd ? "[name].[contenthash].js" : "[name].js",
    },
    optimization: {
        minimizer: isProd ? [new UglifyJsPlugin({sourceMap: true})] : [],
        runtimeChunk: "single",
        splitChunks: {
            cacheGroups: {
                vendor: {
                    test: /[\\/]node_modules[\\/]/,
                    name: "vendors",
                    chunks: "all"
                }
            }
        }
    }
};
