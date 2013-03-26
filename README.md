nodegit
=======

> Node.js libgit2 bindings

**v0.0.78** [![Build
Status](https://travis-ci.org/tbranyen/nodegit.png)](https://travis-ci.org/tbranyen/nodegit)

Maintained by Tim Branyen [@tbranyen](http://twitter.com/tbranyen) and Michael
Robinson [@codeofinterest](http://twitter.com/codeofinterest), with help from
[awesome
contributors](https://github.com/tbranyen/nodegit/contributors)!

Contributing
------------

Nodegit aims to eventually provide native asynchronous bindings for as much of
libgit2 as possible, but we can't do it alone!

We welcome pull requests, but please pay attention to the following: whether
your lovely code fixes a bug or adds a new feature, please include unit tests
that either prove the bug is fixed, or that your new feature works as expected.
See [running tests](#running-tests)

Unit tests are what makes the Node event loop go around.

Building and installing
-----------------------

### Dependencies ###
To install `nodegit` you need `Node.js`, `python` and `cmake`. To run unit tests you will need to have
`git` installed and accessible from your `PATH` to fetch any `vendor/` addons.

### Easy install (Recommended) ###
This will install and configure everything you need to use `nodegit`.

```` bash
$ npm install nodegit
````

### Mac OS X/Linux/Unix ###

#### Install `nodegit` by cloning source from GitHub and running `node install`: ####

```` bash
$ git clone git://github.com/tbranyen/nodegit.git
$ cd nodegit
$ node install
````

\*Updating to a new version\*

```` bash
$ git pull
$ node install
````

### Windows via Cygwin ###

#### `nodegit` has been compiled and tested to work with the setup required to build and run `Node.js` itself. ####

Instructions on compiling `Node.js` on a Windows platform can be found here:
[https://github.com/ry/node/wiki/Building-node.js-on-Cygwin-(Windows)](https://github.com/ry/node/wiki/Building-node.js-on-Cygwin-%28Windows%29)

API Example Usage
-----------------

### Git Log Emulation ###

#### Convenience API ####

```JavaScript
var git = require("nodegit");

// Read a repository
var git = require('nodegit');

// nodegit follows the error, values ... callback argument convention
git.repo('.git', function(error, repo) {
  if (error) { throw error; }

  // Use the master branch
  repo.branch('master', function(error, branch) {
    if (error) { throw error; }

    // Print out `git log` emulation for each commit in the branch's history
    branch.history().on('commit', function(error, commit) {
      console.log('commit ' + commit.sha);
      console.log(commit.author.name + '<' + commit.author.email + '>');
      console.log(new Date(commit.time * 1000));
      console.log("\n");
      console.log(commit.message);
      console.log("\n");
    });
  });
});
```

#### Raw API ####

```` javascript
var git = require('nodegit').raw;

// Create instance of Repo constructor
var repo = new git.Repo();

// Read a repository
repo.open('.git', function(err) {
  // Err is an integer, success is 0, use strError for string representation
  if (err) {
    var error = new git.Error();
    throw error.strError(err);
  }

  // Create instance of Ref constructor with this repository
  var ref = new git.Ref(repo);

  // Find the master branch
  repo.lookupRef(ref, '/refs/heads/master', function(err) {
    if (err) {
      var error = new git.Error();
      throw error.strError(err);
    }

    // Create instance of Commit constructor with this repository
    var commit = new git.Commit(repo);

    // Create instance of Oid constructor
    var oid = new git.Oid();

    // Set the oid constructor internal reference to this branch reference
    ref.oid(oid);

    // Lookup the commit for this oid
    commit.lookup(oid, function(err) {
      if (err) {
        var error = new git.Error();
        throw error.strError(err);
      }

      // Create instance of RevWalk constructor with this repository
      var revwalk = new git.RevWalk(repo);

      // Push the commit as the start to walk
      revwalk.push(commit);

      // Recursive walk
      function walk() {
        // Each revision walk iteration yields a commit
        var revisionCommit = new git.Commit(repo);

        revwalk.next(revisionCommit, function(err) {
          // Finish recursion once no more revision commits are left
          if (err) { return; }

          // Create instance of Oid for sha
          var oid = new git.Oid();

          // Set oid to the revision commit
          revisionCommit.id(oid);

          // Create instance of Sig for author
          var author = new git.Sig();

          // Set the author to the revision commit author
          revisionCommit.author(author);

          // Convert timestamp to milliseconds and set new Date object
          var time = new Date( revisionCommit.time() * 1000 );

          // Print out `git log` emulation
          console.log(oid.toString( 40 ));
          console.log(author.name() + '<' + author.email() + '>');
          console.log(time);
          console.log('\n');
          console.log(revisionCommit.message());
          console.log('\n');

          // Recurse!
          walk();
        });
      }

      // Initiate recursion
      walk():
    });
  });
});
````

Running tests
-------------

__`nodegit` library code is written adhering to a modified `JSHint`. Run these checks with `make lint` in the project root.__

__To run unit tests ensure to update the submodules with `git submodule update --init` and install the development dependencies nodeunit and rimraf with `npm install`.__

Then simply run `npm test` in the project root.

Documentation
------------------------

Recent documentation may be found here: [`nodegit` documentation](http://tbranyen.github.com/nodegit/)

__`nodegit` native and library code is documented to be built with `Natural Docs`.__

To create the documentation, `cd` into the `nodegit` dir and run the following:
    $ cd nodegit
    $ make doc

The documentation will then generate in the `doc/` subfolder as HTML.

Release information
-------------------

__Can keep track of current method coverage at: [http://bit.ly/tb_methods](http://bit.ly/tb_methods)__

### v0.0.7: ###
    * Updated to work with Node ~0.8.
    * More unit tests
    * Added convenience build script
    * Locked libgit2 to version 0.15.0

### v0.0.6: ###
    * Updated to work with Node ~0.6.

### v0.0.5: ###
    * Added in fast Buffer support.
    * Blob raw write supported added, no convenience methods yet...
    * Updated libgit2 to version 0.12.0

### v0.0.4: ###
    * Many fixes!
    * Blob raw write supported added, no convenience methods yet...
    * Updated libgit2 to version 0.12.0

### v0.0.3: ###
    * More documented native source code
    * Updated convenience api code
    * More unit tests
    * Updated libgit2 to version 0.11.0
    * Windows Cygwin support! *albeit hacky*

### v0.0.2: ###
    * More methods implemented
    * More unit tests
    * More API development
    * Tree and Blob support
    * Updated libgit2 to version 0.8.0

### v0.0.1: ###
    * Some useful methods implemented
    * Some unit tests
    * Some documented source code
    * Useable build/code quality check tools
    * Node.js application that can be configured/built/installed via source and NPM
    * An API that can be easily extended with convenience methods in JS
    * An API that offers a familiar clean syntax that will make adoption and use much more likely
    * Open for public testing
    * GitHub landing page
    * Repo, Oid, Commit, Error, Ref, and RevWalk support
    * Built on libgit2 version 0.3.0

Getting involved
----------------

If you find this project of interest, please document all issues and fork if
you feel you can provide a patch.  Testing is of huge importance; by simply
running the unit tests on your system and reporting issues you can contribute!

__Before submitting a pull request, please ensure both that you've added unit
tests to cover your shiny new code, and that all unit tests and lint checks
pass.__