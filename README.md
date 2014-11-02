epd
=========

epd is a library for controlling Davicom E-paper driver serise.
Now, support DM130036 and DM130120.


Version
----

1.0


Initialize
--------------

Using EPD to configure the object with driver model, clock pin  and data pin
```sh
EPD epd(0,9,8);

```
 - epd => object
 - 0 => driver model
   - 0 => DM130036
   - 1 => DM130120
 - 9 => clock pin
 - 8 => data pin


Functions
--------------
 - writeRegister(byte _nodeID, byte _address, byte _data);
 - allWhite(byte _nodeID);
 - allBlack(byte _nodeID);
 - powerSaving(byte _nodeID);


License
----

GPL V2