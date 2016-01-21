var livolo = require("../pi-livolo"),
should = require("should"),
fs = require("fs");

describe("pi-livolo", function() {
         describe(".open", function() {
                  it("should open without errors", function(done) {
                     livolo.open(12, function(err) {
                               should.not.exist(err);
                               done();
                               });
                     });
                  
                  it("should throw an error if the pin is invalid", function() {
                     try {
                     livolo.open(1);
                     } catch(e) {
                     e.should.exist;
                     }
                     });

         describe(".close", function() {
                  it("should close an open pin", function(done) {
                     livolo.close(12, done);
                     });
                  });
         
         describe(".write", function() {
                  it("should write the value of the pin", function(done) {
                     livolo.write(31565, 8, function(err) {
                        should.not.exist(err);
                     });
                  });
         
         after(function(done) {
               livolo.close(1, done);
               });
         });