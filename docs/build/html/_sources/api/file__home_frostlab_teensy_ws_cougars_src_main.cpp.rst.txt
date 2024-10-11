
.. _file__home_frostlab_teensy_ws_cougars_src_main.cpp:

File main.cpp
=============

|exhale_lsh| :ref:`Parent directory <dir__home_frostlab_teensy_ws_cougars_src>` (``/home/frostlab/teensy_ws/cougars/src``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS


This node is the micro-ROS node for the CougUV. It controls the actuators (servos and thruster) and reads the sensor data (battery, leak, and pressure). The node communicates with the Raspberry Pi over micro-ROS. 



.. contents:: Contents
   :local:
   :backlinks: none

Definition (``/home/frostlab/teensy_ws/cougars/src/main.cpp``)
--------------------------------------------------------------


.. toctree::
   :maxdepth: 1

   program_listing_file__home_frostlab_teensy_ws_cougars_src_main.cpp.rst



Detailed Description
--------------------

Nelson Durrant 

September 2024

Subscribes:

- kinematics/command (frost_interfaces/msg/UCommand) Publishes:

- battery_status (frost_interfaces/msg/BatteryStatus)

- leak_status (frost_interfaces/msg/LeakStatus)

- pressure_data (sensor_msgs/msg/FluidPressure) 






Includes
--------


- ``Servo.h``

- ``SoftwareSerial.h``

- ``Wire.h``

- ``battery_pub.h`` (:ref:`file__home_frostlab_teensy_ws_cougars_include_battery_pub.h`)

- ``frost_interfaces/msg/u_command.h``

- ``leak_pub.h`` (:ref:`file__home_frostlab_teensy_ws_cougars_include_leak_pub.h`)

- ``pressure_pub.h`` (:ref:`file__home_frostlab_teensy_ws_cougars_include_pressure_pub.h`)






Enums
-----


- :ref:`exhale_enum_main_8cpp_1aa19be6305a5a4485e1e70de70ed7d677`


Functions
---------


- :ref:`exhale_function_main_8cpp_1affc39a1bb3628c29f361ce25b8460992`

- :ref:`exhale_function_main_8cpp_1a98ff7fd3dd310e11aac835bf0632b4bc`

- :ref:`exhale_function_main_8cpp_1a6ebcd257d8ca3d30c19a587407dcc007`

- :ref:`exhale_function_main_8cpp_1a6ec56d5719b6286a3f5fd83f5b26eddd`

- :ref:`exhale_function_main_8cpp_1a1ba932e26c7eb955eb2ad83e9f4adaf1`

- :ref:`exhale_function_main_8cpp_1afe461d27b9c48d5921c00d521181f12f`

- :ref:`exhale_function_main_8cpp_1aea35450bc9674e2b231d84e35a7f6c17`

- :ref:`exhale_function_main_8cpp_1a1d49f4ddd16b1a0b5e16db4d173a93fa`

- :ref:`exhale_function_main_8cpp_1aee2d2b4de37abd0aec9d441d54069678`

- :ref:`exhale_function_main_8cpp_1a4fc01d736fe50cf5b977f755b675f11d`


Defines
-------


- :ref:`exhale_define_main_8cpp_1aee417ef99c9c3a59c7dd2e1112e7916b`

- :ref:`exhale_define_main_8cpp_1ad4455691936f92fdd6c37566fc58ba1f`

- :ref:`exhale_define_main_8cpp_1ae41566ad7730a2bd95ff58f839c43f6f`

- :ref:`exhale_define_main_8cpp_1a21c9a6dd50dac33eded62f5230da33e3`

- :ref:`exhale_define_main_8cpp_1af5f126c36ffc52d539b232facd644531`

- :ref:`exhale_define_main_8cpp_1af605ad2cc80f8f9a7ded74009b24c6d5`

- :ref:`exhale_define_main_8cpp_1a813d6b208edd662faa8b100b8e6b4369`

- :ref:`exhale_define_main_8cpp_1ab9aa5b56d984e5fee1b12cdfce5265f5`

- :ref:`exhale_define_main_8cpp_1a72262e754adf7baf244f9fa47c15e1d2`

- :ref:`exhale_define_main_8cpp_1ad6b8db87712c71951ab67e14182a29b7`

- :ref:`exhale_define_main_8cpp_1a942aaee1e0f53df83a7cae110d727ff0`

- :ref:`exhale_define_main_8cpp_1ad682458c5f8737f6804982f44b514ad5`

- :ref:`exhale_define_main_8cpp_1ab3935ba9b16dcda80eca9c91dc4a9a19`

- :ref:`exhale_define_main_8cpp_1a487166e197ff871feae6f13dc63ec68c`

- :ref:`exhale_define_main_8cpp_1a5bd07a795448af3f61f1f6de06463445`

- :ref:`exhale_define_main_8cpp_1a6767b1b87d2607586edac4ccc264a1ca`

- :ref:`exhale_define_main_8cpp_1ab4553be4db9860d940f81d7447173b2f`

- :ref:`exhale_define_main_8cpp_1aff9a498fcc65172b5a88f268f4e6bdc8`

- :ref:`exhale_define_main_8cpp_1a78a73a2407c0d63cd1810dc4439bb344`

- :ref:`exhale_define_main_8cpp_1a84f683adc12593607818902c067443bf`

- :ref:`exhale_define_main_8cpp_1a54fb2e697d06412f1df1f246528f98db`

- :ref:`exhale_define_main_8cpp_1a2358bf36e4130f9192ca22b04564bf78`

- :ref:`exhale_define_main_8cpp_1aa2844b520f3e69eb56edde7e6bdb8ee0`

- :ref:`exhale_define_main_8cpp_1a8b06d2e9648501088d9fcc2b3c714690`

- :ref:`exhale_define_main_8cpp_1a86ab1083117316547bd3ffffcac49126`

- :ref:`exhale_define_main_8cpp_1a6577fea59e1409dfe839dc5742fb03f2`

- :ref:`exhale_define_main_8cpp_1a6c6eafa621ccb8e488cb2b90c38b9bb4`

- :ref:`exhale_define_main_8cpp_1a4bef1bdab442f2f070157bb0d21f321c`

- :ref:`exhale_define_main_8cpp_1adda173dd4a2880eaa2832e6e8795cd11`

- :ref:`exhale_define_main_8cpp_1ac5a3ff6febcb0c6dbf98c74e8fff07f4`

- :ref:`exhale_define_main_8cpp_1ae7f6917411b54a5c51ba9baba7c70798`


Variables
---------


- :ref:`exhale_variable_main_8cpp_1acc1518bac7ff8bf75ac11589055d7578`

- :ref:`exhale_variable_main_8cpp_1ababf2ce58bfce2468b7dc38dce6493f7`

- :ref:`exhale_variable_main_8cpp_1adf673e6d48bde321870e7389d0fd3a2b`

- :ref:`exhale_variable_main_8cpp_1a38adf196df51513c6d7c15150e91c69d`

- :ref:`exhale_variable_main_8cpp_1a864bc8f364fa8dd00b7265f7fd39ce84`

- :ref:`exhale_variable_main_8cpp_1a69715aba74526f43c76f3c4576d1d776`

- :ref:`exhale_variable_main_8cpp_1aafa81c979da110a109ca38521e8bb42e`

- :ref:`exhale_variable_main_8cpp_1ad282831427871253f8f6b74232d5e425`

- :ref:`exhale_variable_main_8cpp_1ac559bc920cd1ad5655c740616594b1b5`

- :ref:`exhale_variable_main_8cpp_1ac773ef935f8c08f2f6d2b5c358ecea29`

- :ref:`exhale_variable_main_8cpp_1ab232a7b77d64adaf0c683ca167311ab7`

- :ref:`exhale_variable_main_8cpp_1accdcea30823dd428e6aea6c4e8657ba6`

- :ref:`exhale_variable_main_8cpp_1ab69e3b0b5d335275c7e2da94c0886b1c`

- :ref:`exhale_variable_main_8cpp_1a75ffc6c4c2d6702a3f379bf2513ac8e1`

- :ref:`exhale_variable_main_8cpp_1aa56f777b40743798ee0726ba4ded17a9`

- :ref:`exhale_variable_main_8cpp_1ad7ca784e9d6a36f1a990ccef47340149`

