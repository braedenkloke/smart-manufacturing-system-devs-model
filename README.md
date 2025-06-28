# Smart Manufacturing System DEVS Model 
[DEVS](https://en.wikipedia.org/wiki/DEVS) model and [Cadmium](https://devssim.carleton.ca/) simulation of the [smart manufacturing](https://en.wikipedia.org/wiki/Smart_manufacturing) system studied by [Friederich and Lazarova-Molnar (2024)](https://journals.sagepub.com/doi/full/10.1177/00375497241302866?casa_token=TZrSVjFZ1-YAAAAA%3AI2L_IkiDduYKh8NvE_x07_OGoQNQjjB3_x00TFNuzlrwdu8NNnzc1HG7uaIE3aYi7RN6gynVvqpXbQ).

# Usage
With [Cadmium](https://devssim.carleton.ca/) installed, run
```
source build_sim.sh
./bin/smart_manufacturing_system
```

Your output should be,
```
INITIAL STATES
0;1;mes;;State Log: Idle
0;2;customer;;State Log: Waiting
START SIMULATION
0;2;customer;;State Log: Placing Order
0;1;mes;;State Log: Initiating New Order
0;2;customer;placeOrderEventPort;Event Log: orderID: 1
0;2;customer;;State Log: Waiting
0;1;mes;newOrderEventPort;Event Log: orderID: 1
...
3;2;customer;;State Log: Out of Orders
3;1;mes;newOrderEventPort;Event Log: orderID: 4
3;1;mes;;State Log: Idle
END SIMULATION
FINAL STATES
3;1;mes;;State Log: Idle
3;2;customer;;State Log: Out of Orders
```
The above output has been modified slightly to improve readability.
The headers are not included in the Cadmium logs.
On the command line, 
state logs are printed as yellow while event logs are printed as green.

For how to supply your own order inputs, 
refer to the README file in the [in/orders](in/orders) directory.

## Remote Simulation
Alternatively, you can run the simulation remotely on the [DEVSim servers](https://devssim.carleton.ca/). 

# Acknowledgements
- [Sanja Lazarova-Molnar](https://lazarova-molnar.net/) and the [SYDSEN Research Group](https://sydsen.aifb.kit.edu/) 
- [Gabriel Wainer](https://www.sce.carleton.ca/faculty/wainer/doku.php) and the [ARSLab](https://arslab.sce.carleton.ca/) 
- [Mitacs](https://www.mitacs.ca/our-programs/globalink-research-award/)

# See Also
- [Friederich, J. (2023). *Data-Driven Assessment of Reliability for Cyber-Physical Production Systems*.](https://portal.findresearcher.sdu.dk/en/publications/data-driven-assessment-of-reliability-for-cyber-physical-producti)
- [Cadmium Project Template](https://github.com/Sasisekhar/blank_project_rt)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [hackergrrl/art-of-readme](https://github.com/hackergrrl/art-of-readme)

# License
[MIT](https://choosealicense.com/licenses/mit/)
