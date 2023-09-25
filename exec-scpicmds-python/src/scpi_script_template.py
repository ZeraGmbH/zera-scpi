from src.scpi_scripting_interface import IScpiScripting
from src.logging_handler import LoggingColor, LoggingStyle


class ScpiScript(IScpiScripting):
    def run(self) -> int:
        self.send("MEASURE:FFT1:UL1?")
        self.wait_for_opc()
        stb = int(self.send("*STB?")[0])
        if stb == 0:
            self.log("STB = 0", LoggingColor.GREEN)
        else:
            self.log(f"STB = {stb}", color=LoggingColor.RED, style=LoggingStyle.BOLD)
            return 100  # exit code

