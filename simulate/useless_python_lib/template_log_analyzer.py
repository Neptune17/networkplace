from abc import ABC, abstractmethod

class TemplateLogAnalyzer(ABC):
    def __init__(self, name : str):
        self.name : str = name
        self.match_prefix : str = "{\"name\":\"" + name + "\","

    @abstractmethod
    def need_update(self, input_file_modify_time : float) -> bool:
        pass

    @abstractmethod
    def parse_feature(self, str_feature_log : str):
        pass

    @abstractmethod
    def dump_feature(self):
        pass