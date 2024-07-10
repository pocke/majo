module Majo
  class AllocationInfo
    def inspect
      "#<Majo::AllocationInfo #{object_class_path} #{class_path}##{method_id} #{path}:#{line} gen#{generation}>"
    end
  end
end
