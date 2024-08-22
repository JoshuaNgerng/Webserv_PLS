const express = require("express")
const app = express()
app.get("/", (req, res) => {
    res.send("I am an endpoint")
})
app.listen(7777, () => {
    console.log("listen on port 7777")
})