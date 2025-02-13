from typing import Tuple
import numpy as np


"""
The python binding of the C++ Algorithms of Dustear
"""


class Algorithms:
    def __init__(self, config_path: str = "config.json") -> None:
        ...

    def lsq_fit(self, pulse: np.ndarray[np.float64], amplitude: np.float64) -> Tuple[np.float64, np.ndarray]:
        """
        Fits with an estimation amplitude the model
        
        f(x_1, x_2, x_3, x_4)_t = -x_1 exp(-(t-x_4)/x_2*1e6)sin(2pix_3(t-x_4)*1e3)  -x_1 * k_A exp(-(t-x_4)/(k_t * x_2)*1e6)sin(2pix_3(t-x_4) * k_f * 1e3) 
        
        Parameters
        ----------
        x : float (double)
            First guess for the amplitude

        Returns
        -------
        np.ndarray: The fitted parameters

        """
    
    def average_cost(self, pulses: np.ndarray[np.ndarray[np.float64]], k_A: np.float64, k_tau: np.float64, k_f: np.float64) -> np.float64:
        """
        Iterates over all pulses, calculcated for each a LSQ fit and averages the cost of each.
        This function is intented to be used as a cost-function for bayes-optimization to find
        propert constants k_A, k_tau, k_f.
        """

    def execute_offline(self, file_name: str) -> None:
        ...

    @property
    def file_prefix(self) -> str:
        ...
    
    @file_prefix.setter
    def file_prefix(self, file_prefix: str) -> None:
        ...

    @property
    def destination_folder(self) -> str:
        ...
    
    @destination_folder.setter
    def destination_folder(self, destination_folder: str) -> None:
        ...
