class FileHelper:
    @staticmethod
    def get_filename_from_class_and_method(c: type, m: callable):
        return c.__class__.__name__ + "__" + m.__name__