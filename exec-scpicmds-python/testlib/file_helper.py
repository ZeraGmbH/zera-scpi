class FileHelper:
    @staticmethod
    def get_filename_from_class_and_method(class_type: type, method: callable):
        return class_type.__class__.__name__ + "__" + method.__name__
