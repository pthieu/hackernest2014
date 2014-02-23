exports.landing = function (req, res) {
    res.sendfile('public/index.html');
}

exports.create = function (req, res) {
    res.sendfile('public/create.html');
}